#include <stdlib.h>
#include <string.h>
#include "hash_defs.h"
/**
 * Hash function compendium.
 * 
 * Taken from http://www.cse.yorku.ca/~oz/hash.html,
 * https://stackoverflow.com/questions/7666509/hash-function-for-string
 * 
 */

unsigned long hashDjb2(Hashkey key) {
    unsigned long hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + *key++; /* hash * 33 + c */
    }
    return hash;
}

unsigned long sdbm(Hashkey key) {
    unsigned long hash = 0;
    while (*key) {
        hash = *key++ + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

unsigned long jenkins1AtTime(Hashkey key) {
    unsigned long hash = 0;
    while (*key) {
        hash += *key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned long rol(const unsigned long value, int shift);

unsigned long rolHash(Hashkey input) { 
    unsigned long result = 0x55555555;
    while (*input) { 
        result ^= *input++;
        result = rol(result, 5);
    }
    return result;
}

/**
 * rol() architecture-neutral rotate left function.
 * https://stackoverflow.com/questions/10134805/bitwise-rotate-left-function
 * https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c
 * 
 */
unsigned long rol(const unsigned long value, int shift) {
    if ((shift &= sizeof(value)*8 - 1) == 0)
      return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

// hash function - return hash of given string key modulo number of buckets.
// (based on an algorithm by Sedgewick)
unsigned long sdgwckHash(Hashkey key) {
	unsigned int hash_val = 0;
	int len = strlen(key);
	for (int i = 0; i < len; i++) {
		hash_val = (DEFAULT_NUM_BUCKETS * hash_val + key[i]);
	}
	return hash_val;
}