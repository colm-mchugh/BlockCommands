#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "commands_big.h"
#include "hash.h"
#include "assert.h"

#define DEFAULT_NUM_BUCKETS 4783

int main() {
    int i;
    int commandCount = sizeof (CommandList) / sizeof (CommandList[0]);
    unsigned int hashBuckets[DEFAULT_NUM_BUCKETS];
    bzero(hashBuckets, sizeof (unsigned int) * DEFAULT_NUM_BUCKETS);
    for (int j = 0; j < DEFAULT_NUM_BUCKETS; j++) {
        assert(hashBuckets[j] == 0);
    }
    int inserts = 0;
    int collisions = 0;
    int unused = 0;
    int used = 0;
    for (i = 0; i < commandCount; i++) {
        if (CommandList[i].value == NULL) {
            printf("%s: %s\n", CommandList[i].cmd, CommandList[i].key);
        } else {
            printf("%s: %s %s\n", CommandList[i].cmd, CommandList[i].key, CommandList[i].value);
        }
        if (strcmp(CommandList[i].cmd, "INS") == 0) {
            unsigned int h = rolHash(CommandList[i].key) % DEFAULT_NUM_BUCKETS;
            if (hashBuckets[h] > 0) {
                collisions++;
            }
            hashBuckets[h]++;
            inserts++;
        }
    }
    for (unsigned int i = 0; i < DEFAULT_NUM_BUCKETS; i++) {
        printf("Bucket: %d, entries: %d\n", i, hashBuckets[i]);
        if (hashBuckets[i] == 0) {
            unused++;
        } else {
            used++;
        }
    }
    printf("Inserts: %d, Collisions %d, Unused %d, Used %d\n", inserts, collisions, unused, used);
    return (0);
}
