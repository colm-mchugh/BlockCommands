/* 
 * File:   BEHash.h
 * Author: colm_mchugh
 *
 * Created on July 13, 2017, 8:46 AM
 */

#ifndef BEHASH_H
#define	BEHASH_H

#include "BEList.h"
#include "hash.h"

// Basic hashtable implementation - uses BEList for 
// each hash bucket


/*  BEHash provides a hashtable of Block Entry structures.
 *  It is implemented using a fixed number of buckets. 
 *  Each bucket is a Block Entry list, to resolve collisions
 *  (different keys hashing to the same offset in BEHash.buckets)
 */
typedef struct _hashtable {
		BEList** buckets;
                f_compare key_cmp;
                f_hash  key_hash;
		int num_buckets;
                int num_entries;
} BEHash;

// Allocate a BEHash
BEHash *createBEHash(f_compare);

// Delete a BEHash, apply deleter function to elements in the hash
void deleteHash(BEHash**, deleter);

// Add the given key - block entry to a BEHash
void AddBE(BEHash*, Hashkey, BlockEntry*);

// Return the block entry for the given hashkey
BlockEntry *LookupBE(BEHash *, Hashkey);

// Remove the given key - block entry from the BEHash
void DeleteBE(BEHash*, Hashkey, BlockEntry*);


#endif	/* BEHASH_H */

