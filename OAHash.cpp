#include "OAHash.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define DELETED ((BlockEntry*)0x1)

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
        if (h->htable[i] != DELETED && (h->cmp_keys)(key, h->htable[i]->block + h->htable[i]->offset)) {
            found = true;
            break;
        }
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
    while (i < h->size && h->htable[i] != NULL && h->htable[i] != DELETED 
            && !((h->cmp_keys)(key, h->htable[i]->block + h->htable[i]->offset))) {
        i++;
        h->collisions++;
    }
    assert(i < h->size);
    h->htable[i] = data;
    h->num_entries++;
    h->inserts++;
    assert(h->num_entries <= h->size);
}

BlockEntry* DeleteOA(OAHash* h, Hashkey key) {
    unsigned int i = findIndexOf(h, key);
    BlockEntry* data = NULL;
    if (i != -1) {
        data = h->htable[i];
        h->htable[i] = DELETED;
        h->num_entries--;
        h->deletes++;
    }
    return data;
}

void deleteOAHash(OAHash* h) {
    int delete_holes = 0;
    for (int i = 0; i < h->size; i++) {
        if (h->htable[i] == DELETED) {
            delete_holes++;
        }
    }
    printf("Number of insert collisions: %d\n", h->collisions);
    printf("Number of inserts: %d\n", h->inserts);
    printf("Number of deletes: %d\n", h->deletes);
    printf("Number of delete holes: %d\n", delete_holes);
    for (int i = 0; i < h->size; i++) {
        if (h->htable[i] != NULL && h->htable[i] != DELETED) {
            BlockEntry *b = h->htable[i];
            free(b);
            h->htable[i] = NULL;
        }
    }
}