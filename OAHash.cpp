#include "OAHash.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

OAHash *createOAHash(f_compare cmp_keys) {
    OAHash *newHash = (OAHash*) malloc(sizeof (OAHash));
    newHash->htable = (BlockEntry**) malloc(sizeof (BlockEntry**) * DEFAULT_NUM_BUCKETS);
    newHash->cmp_keys = cmp_keys;
    newHash->hash_key = hashDjb2;
    newHash->num_entries = 0;
    newHash->collisions = 0;
    newHash->inserts = 0;
    newHash->deletes = 0;
    newHash->size = DEFAULT_NUM_BUCKETS;
    return newHash;
}

unsigned int findIndexOf(OAHash *h, Hashkey key) {
    unsigned int idx = (h->hash_key)(key) % h->size;
    unsigned int i = idx;
    bool found = false;
    while (i < h->size && h->htable[i] != NULL) {
        if ((h->cmp_keys)(key, h->htable[i]->block + h->htable[i]->offset)) {
            found = true;
            break;
        }
        h->collisions++;
        i++;
    }
    return (found ? i : -1);
}

// lookup - determine which bucket; determine if there
// is a value with the given key in that bucket 

BlockEntry *LookupOA(OAHash *h, Hashkey key) {
    unsigned int i = findIndexOf(h, key);
    return (i != -1 ? h->htable[i] : NULL);
}

// add key - BlockEntry to the appropriate bucket

void AddOA(OAHash *h, Hashkey key, BlockEntry* data) {
    unsigned int idx = (h->hash_key)(key) % h->size;
    unsigned int i = idx;
    while (i < h->size && h->htable[i] != NULL && !((h->cmp_keys)(key, h->htable[i]->block + h->htable[i]->offset))) {
        i++;
        h->collisions++;
    }
    assert(i < h->size);
    h->htable[i] = data;
    h->num_entries++;
    h->inserts++;
    //printf("hash is %d, insert index is %d\n", idx, i);
    assert(h->num_entries <= h->size);
}

BlockEntry* DeleteOA(OAHash* h, Hashkey key) {
    unsigned int i = findIndexOf(h, key);
    BlockEntry* data = NULL;
    if (i != -1) {
        data = h->htable[i];
        h->htable[i] = NULL;
        h->num_entries--;
        h->deletes++;
    }
    return data;
}

void deleteOAHash(OAHash* h) {
    // toDO
    printf("Number of collisions: %d\n", h->collisions);
    printf("Number of inserts: %d\n", h->inserts);
    printf("Number of deletes: %d\n", h->deletes);

}