/* 
 * File:   OAHash.h
 * Author: colm_mchugh
 *
 * Created on July 25, 2017, 4:17 PM
 */

#ifndef OAHASH_H
#define	OAHASH_H

#include "commands_data.h"
#include "hash_defs.h"

typedef struct _hashtable {
		BlockEntry** htable;
		char *block;
                f_compare cmp_keys;
                f_hash hash_key;
                int size;
		int num_entries;
                int collisions;
                int inserts;
                int deletes;
} OAHash;

// Allocate a OAHash
OAHash *createOAHash(f_compare);

// Delete a OAHash
void deleteOAHash(OAHash*);

// Add the given key - block entry to a OAHash
void AddOA(OAHash*, Hashkey, BlockEntry*);

// Return the block entry for the given hashkey
BlockEntry *LookupOA(OAHash *, Hashkey);

// Remove the given key - block entry from the OAHash
BlockEntry *DeleteOA(OAHash*, Hashkey);

#endif	/* OAHASH_H */

