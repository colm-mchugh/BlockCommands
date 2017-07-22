/* 
 * File:   commands_interpreter.h
 * Author: colm_mchugh
 *
 * Created on July 13, 2017, 8:46 AM
 */

#ifndef BLOCK_MANAGER_H
#define	BLOCK_MANAGER_H

#include "commands_data.h"
#include "BEHash.h"

/**
 BlockManager provides the fundamental functionality for executing block commands.
 * Structurally, it consists of:
 *  - The BlockData, the actual block and its metadata.
 *  - A table of block entries, i.e a mapping from key to block entry.
 *  - The most recently allocated block entry. This is used as a list
 *    of block entries, from most recently created to least.
 * 
 * Functionally, it consists of:
 *  - create block manager - creates a block manager with an empty block
 *    and empty block entry table
 *  - executeCommand - executes the given command
 *  - delete block manager - deallocates all memory referenced by the 
 *    block manager
 */
typedef struct _blck_manager {
    BlockData *blockData;
    BEHash *blockEntries;
    BlockEntry *lastAllocatedBlock;
} BlockManager;

#define BLOCK_SIZE 4096

BlockManager *createBlockManager(int block_size);

void executeCommand(BlockManager *interprtr, char *cmd, char* key, char* value);

void deleteBlockManager(BlockManager *interprtr);

#endif	/* BLOCK_MANAGER_H */

