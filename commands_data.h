/* 
 * File:   commands_data.h
 * Author: colm_mchugh
 *
 * Created on July 13, 2017, 8:30 AM
 */

#ifndef COMMANDS_DATA_H
#define	COMMANDS_DATA_H

/** 
 * BlockData is the basic data structure for managing a block. 
 * Structurally, it consists of: 
 *  - the actual block data (the block pointer)
 *  - the overall size of the block (size)
 *  - the free space left in the block (freeSize)
 *  - next available index in block where a value can be placed (nextFreeOffset)
 */
typedef struct _blockData {
    char *block;
    int size;
    int freeSize;
    int nextFreeOffset;
} BlockData;

/**
 * BlockEntry provides metadata about key and its row:
 * - the starting position of the row in the block (offset)
 * - block + offset: | Key | value length | value |
 * - the previously allocated block entry (nextOldest) 
 * - the next allocated block entry (nextNewest)
 * 
 * BlockEntries are maintained in order of creation by the 
 * BlockManager. 
 */
typedef struct _blockEntry {
    char *block;
    int offset;
    int length;
    struct _blockEntry *nextOldest;
    struct _blockEntry *nextNewest;
} BlockEntry;

#define KEY_LEN 5

#define KEY_SIZE KEY_LEN + 1

bool key_compare(char *, char*);

#endif	/* COMMANDS_DATA_H */

