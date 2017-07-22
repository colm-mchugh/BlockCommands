#include "block_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * Create an initialized Block manager. 
 * An initialized Block manager consists of:
 *  - An empty Block Entry table
 *  - An empty Block; the free size is equal to the block size
 * @return 
 */
BlockManager *createBlockManager(int block_size) {
    BlockManager *new_blck_mgr = (BlockManager*) malloc(sizeof (BlockManager));
    new_blck_mgr->blockEntries = createHash();
    BlockData *bd = (BlockData*) malloc(sizeof (BlockData));
    bd->block = (char*) malloc(sizeof (char) * block_size);
    bd->freeSize = block_size;
    bd->nextFreeOffset = 0;
    bd->size = block_size;
    new_blck_mgr->blockData = bd;
    new_blck_mgr->lastAllocatedBlock = NULL;
    return new_blck_mgr;
}

void printState(BlockManager *bm) {
    printf("BM: block size=%d, free size=%d, offset=%d, #entries=%d\n", bm->blockData->size, bm->blockData->freeSize, bm->blockData->nextFreeOffset, bm->blockEntries->num_entries);
    assert(bm->blockData->size - bm->blockData->nextFreeOffset == bm->blockData->freeSize);
}



// Insert the given key, value if the key is not present and there
// is enough space. Please refer to the comments in the code for how
// the key - value is inserted.

void doInsert(BlockManager *blckManager, char* key, char* value) {
    // Is the key already present? 
    if (LookupBE(blckManager->blockEntries, key) != NULL) {
        printf("Key %s exists\n", key);
        return;
    }
    int sz = strlen(value);
    // Is there enough space?
    if (blckManager->blockData->freeSize - sz < 0) {
        printf("Block is too full to insert %s\n", key);
        return;
    }
    // There is enough space - put the value at the next available 
    // place in the block:
    char *base = blckManager->blockData->block;
    int offset = blckManager->blockData->nextFreeOffset;
    strncpy(base + offset, value, sz);

    // Add a new block entry for this insertion
    BlockEntry *newBE = (BlockEntry*) malloc(sizeof (BlockEntry));
    strcpy(newBE->key, key);
    newBE->length = sz;
    newBE->offset = offset;

    // The new block entry now becomes the last allocated block
    // of the block manager
    newBE->nextOldest = blckManager->lastAllocatedBlock;
    newBE->nextNewest = NULL;
    if (blckManager->lastAllocatedBlock != NULL) {
        blckManager->lastAllocatedBlock->nextNewest = newBE;
    }
    blckManager->lastAllocatedBlock = newBE;

    // Put the new block entry in the block manager's entries table
    AddBE(blckManager->blockEntries, key, newBE);

    // Update the sizing metadata in blockData:
    blckManager->blockData->freeSize -= sz;
    blckManager->blockData->nextFreeOffset += sz; // next entry will go after this one 
    printState(blckManager);
}

// Print the row for the given key if the key exists in the block manager's
// block entry table

void doSelect(BlockManager *blckManager, char* key) {
    BlockEntry *entry = LookupBE(blckManager->blockEntries, key);
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
    } else {
        // There is a value for the given key in the block: 
        // print its chars 1 by 1, using the block entry's length 
        // to stop..
        char *block = blckManager->blockData->block;
        int entryOffset = entry->offset;
        for (int i = 0; i < entry->length; i++) {
            putchar(block[entryOffset + i]);
        }
        putchar('\n');
    }
}

// Starting at the given offset, copy the block entries starting at the given
// Block Entry down to that offset. This is a memory management utility and is
// used (by doDelete and doUpdate) to plug gaps that may appear in a block.

void doBlockMove(BlockManager *blckManager, int offset, BlockEntry* start) {
    int nextOffset = offset;
    int startPos = (start != NULL ? start->offset : 0);
    char *base = blckManager->blockData->block;
    for (BlockEntry *be = start; be != NULL; be = be->nextNewest) {
        be->offset = nextOffset;
        nextOffset += be->length;
    }
    strncpy(base + offset, base + startPos, nextOffset - offset);
    blckManager->blockData->nextFreeOffset = nextOffset;
}

// Delete the row for the given key, if it is in the Block Manager's entry table

void doDelete(BlockManager *blckManager, char *key) {
    BlockEntry *entry = LookupBE(blckManager->blockEntries, key);
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
    } else {
        // There is a value for the given key in the block: 
        // Adjust the block manager's free size accordingly
        blckManager->blockData->freeSize += entry->length;
        // Remove the entry from the block manager's entry table
        DeleteBE(blckManager->blockEntries, key, entry);
        // Get the next newest block
        BlockEntry *nextNewst = entry->nextNewest;
        // Remove the entry from the order of allocation list:
        if (entry->nextNewest == NULL) {
            // entry == blckManager->lastAllocatedEntry
            blckManager->lastAllocatedBlock = entry->nextOldest;
            blckManager->lastAllocatedBlock->nextNewest = NULL;
        } else {
            nextNewst->nextOldest = entry->nextOldest;
            if (entry->nextOldest != NULL) {
                entry->nextOldest->nextNewest = nextNewst;
            }
        }
        if (nextNewst != NULL) {
            // To maintain contiguous space, we can move the next newest entry,
            // and its subsequent entries (if any), down in the data block.
            doBlockMove(blckManager, entry->offset, nextNewst);
        }
        free(entry);
        printState(blckManager);
    }
}

// Update the given key with the given value if the key is present and there
// is enough space. Please refer to the comments in the code for how
// the update is done.

void doUpdate(BlockManager *blckManager, char* key, char* value) {
    BlockEntry *entry = LookupBE(blckManager->blockEntries, key);
    // Check there is a key to update
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
        return;
    }
    int sz = strlen(value);
    // Verify that there is enough overall free space for this update
    if (blckManager->blockData->freeSize + sz > blckManager->blockData->size) {
        printf("Block is too full to update row to %s\n", value);
        return;
    }
    char *base = blckManager->blockData->block;
    int offset = entry->offset;
    // There will be a gap of size |entry->length - sz| in the block. This can be 
    // plugged by moving newer blocks to the start of this gap.
    doBlockMove(blckManager, offset + sz, entry->nextNewest);
    // Copy the given value into the block starting at the entry's
    // position
    strncpy(base + offset, value, sz);
    // Update free size - could be a -ve adjustment if new value is longer than
    // the previous value
    blckManager->blockData->freeSize += entry->length - sz; 
    entry->length = sz;
    printState(blckManager);
}

// Simple traffic cop function that figures out which function should be 
// called for the given command.

void executeCommand(BlockManager *blckManager, char *cmd, char* key, char* value) {
    if (strcmp(cmd, "INS") == 0) {
        doInsert(blckManager, key, value);
        return;
    }
    if (strcmp(cmd, "SEL") == 0) {
        doSelect(blckManager, key);
        return;
    }
    if (strcmp(cmd, "UPD") == 0) {
        doUpdate(blckManager, key, value);
        return;
    }
    if (strcmp(cmd, "DEL") == 0) {
        doDelete(blckManager, key);
        return;
    }
    printf("Unrecognized command: %s\n", cmd);
}

bool freeBlockEntry(BlockEntry *be) {
    free(be);
    return true;
}

// Free all the memory associated with the given Block Manager

void deleteBlockManager(BlockManager *blckManager) {
    deleteHash(&blckManager->blockEntries, freeBlockEntry);
    free(blckManager->blockData->block);
    free(blckManager->blockData);
    free(blckManager);
}

