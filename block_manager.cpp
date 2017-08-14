#include "block_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef unsigned short len_t;

void set_len(char* block, BlockEntry *b, len_t len) {
    len_t* len_addr = (len_t*)(block + b->offset + KEY_SIZE);
    *len_addr = len;
}

len_t get_len(char* block, BlockEntry *b) {
    len_t* len_addr = (len_t*)(block + b->offset + KEY_SIZE);
    return *len_addr;
}

bool key_compare(Hashkey k1, Hashkey k2) {
    int i = 0;
    while (i < KEY_LEN && k1[i] == k2[i]) {
        i++;
    }
    return i == KEY_LEN;
}

/**
 * Create an initialized Block manager. 
 * An initialized Block manager consists of:
 *  - An empty Block Entry table
 *  - An empty Block; the free size is equal to the block size
 * @return 
 */
BlockManager *createBlockManager(int block_size) {
    BlockManager *new_blck_mgr = (BlockManager*) malloc(sizeof (BlockManager));
    new_blck_mgr->blockEntries = createOAHash(key_compare);
    BlockData *bd = (BlockData*) malloc(sizeof (BlockData));
    bd->block = (char*) malloc(sizeof (char) * block_size);
    new_blck_mgr->blockEntries->block = bd->block;
    bd->freeSize = block_size;
    bd->nextFreeOffset = 0;
    bd->size = block_size;
    new_blck_mgr->blockData = bd;
    new_blck_mgr->lastAllocatedBlock = NULL;
    return new_blck_mgr;
}

void printState(BlockManager *bm) {
    printf("BM: block size=%d, free size=%d, offset=%d, #entries=%d\n", bm->blockData->size, bm->blockData->freeSize, bm->blockData->nextFreeOffset, bm->blockEntries->num_entries);
    //assert(bm->blockData->size - bm->blockData->nextFreeOffset == bm->blockData->freeSize);
}

// Insert the given key, value if the key is not present and there
// is enough space. Please refer to the comments in the code for how
// the key - value is inserted.

void doInsert(BlockManager *blckManager, char* key, char* value) {
    // Is the key already present? 
    if (LookupOA(blckManager->blockEntries, key) != NULL) {
        printf("Key %s exists\n", key);
        return;
    }
    len_t value_size = strlen(value);
    len_t total_size = value_size + KEY_SIZE + sizeof(len_t);
    // Is there enough space?
    if (blckManager->blockData->freeSize - (total_size) < 0) {
        printf("Block is too full to insert %s\n", key);
        return;
    }
    // There is enough space - put the value at the next available 
    // place in the block:
    char *base = blckManager->blockData->block;
    int offset = blckManager->blockData->nextFreeOffset;
    
    // Add a new block entry for this insertion
    BlockEntry *newBE = (BlockEntry*) malloc(sizeof (BlockEntry));
    newBE->offset = offset;
    assert(strlen(key) == KEY_LEN);
    strcpy(base + offset, key);
    set_len(base, newBE, value_size);
    strncpy(base + offset + KEY_SIZE + sizeof(len_t), value, value_size);

    // The new block entry now becomes the last allocated block
    // of the block manager
    newBE->nextOldest = blckManager->lastAllocatedBlock;
    newBE->nextNewest = NULL;
    if (blckManager->lastAllocatedBlock != NULL) {
        blckManager->lastAllocatedBlock->nextNewest = newBE;
    }
    blckManager->lastAllocatedBlock = newBE;

    // Put the new block entry in the block manager's entries table
    AddOA(blckManager->blockEntries, key, newBE);

    // Update the sizing metadata in blockData:
    blckManager->blockData->freeSize -= total_size;
    blckManager->blockData->nextFreeOffset += total_size; // next entry will go after this one 
    printState(blckManager);
}

// Print the row for the given key if the key exists in the block manager's
// block entry table

void doSelect(BlockManager *blckManager, char* key) {
    BlockEntry *entry = LookupOA(blckManager->blockEntries, key);
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
    } else {
        // There is a value for the given key in the block: 
        // print its chars 1 by 1, using the block entry's length 
        // to stop..
        char *block = blckManager->blockData->block;
        int valueOffset = entry->offset + KEY_SIZE + sizeof(len_t);
        int len = get_len(block, entry);
        for (int i = 0; i < len; i++) {
            putchar(block[valueOffset + i]);
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
        len_t len = get_len(base, be);
        be->offset = nextOffset;
        len_t total_size = KEY_SIZE + sizeof(len_t) + len;
        nextOffset += total_size;
    }
    memcpy(base + offset, base + startPos, nextOffset - offset);
    blckManager->blockData->nextFreeOffset = nextOffset;
}

// Delete the row for the given key, if it is in the Block Manager's entry table

void doDelete(BlockManager *blckManager, char *key) {
    BlockEntry *entry = LookupOA(blckManager->blockEntries, key);
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
    } else {
        // There is a value for the given key in the block: 
        // Adjust the block manager's free size accordingly
    	char *base = blckManager->blockData->block;
        len_t len = get_len(base, entry);
        len_t total_size = KEY_SIZE + sizeof(len_t) + len;
        blckManager->blockData->freeSize += total_size;
        // Remove the entry from the block manager's entry table
        DeleteOA(blckManager->blockEntries, key);
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
    BlockEntry *entry = LookupOA(blckManager->blockEntries, key);
    // Check there is a key to update
    if (entry == NULL) {
        printf("Key %s doesn't exist\n", key);
        return;
    }
    len_t new_value_len = strlen(value);
    len_t total_size = KEY_SIZE + sizeof(len_t) + new_value_len;
    // Verify that there is enough overall free space for this update
    if (blckManager->blockData->freeSize + total_size > blckManager->blockData->size) {
        printf("Block is too full to update row to %s\n", value);
        return;
    }
    char *base = blckManager->blockData->block;
    int offset = entry->offset;
    // There will be a gap of size |entry->length - sz| in the block. This can be 
    // plugged by moving newer blocks to the start of this gap.
    doBlockMove(blckManager, offset + total_size, entry->nextNewest);
    // Copy the given value into the block starting at the entry's
    // position
    strncpy(base + offset + KEY_SIZE + sizeof(len_t), value, new_value_len);
    // Update free size - could be a -ve adjustment if new value is longer than
    // the previous value
    len_t curr_value_len = get_len(base, entry);
    blckManager->blockData->freeSize += curr_value_len - new_value_len; 
    set_len(base, entry, new_value_len);
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
    deleteOAHash(blckManager->blockEntries);
    free(blckManager->blockData->block);
    free(blckManager->blockData);
    free(blckManager);
}

