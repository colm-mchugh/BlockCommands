#include "BEHash.h"
#include <stdlib.h>
#include <string.h>

// create a hashtable with num_buckets buckets
BEHash *createBEHash(f_compare compare_keys) {
        int num_buckets = DEFAULT_NUM_BUCKETS;
	BEHash *new_hash = (BEHash*)malloc(sizeof(BEHash));
	new_hash->num_buckets = num_buckets;
	new_hash->buckets = (BEList**)malloc(sizeof(BEList**)*num_buckets);
        new_hash->key_cmp = compare_keys;
        new_hash->key_hash = hashDjb2;
	for(int i = 0; i < new_hash->num_buckets; i++) {
		new_hash->buckets[i] = NULL; // buckets are created on demand
	}
        new_hash->num_entries = 0;
	return new_hash;
}

// add key - BlockEntry to the appropriate bucket
void AddBE(BEHash *h, Hashkey key, BlockEntry* data) {
	int index = (h->key_hash)(key) % h->num_buckets;
	if (h->buckets[index] == NULL) {
		h->buckets[index] = createBEList();
	}
	prependTo(h->buckets[index], data);
        h->num_entries++;
}

// lookup - determine which bucket; determine if there
// is a value with the given key in that bucket 
BlockEntry *LookupBE(BEHash *h, Hashkey key) {
	int index = (h->key_hash)(key) % h->num_buckets;
	BEListNode *n = NULL;
	BEList *bucket = h->buckets[index];
	if (bucket != NULL) {
		n = bucket->head;
                while ((n != NULL) && (h->key_cmp)(n->data->block + n->data->offset, key)) {
                    n = n->next;
                }
	}
	return (n != NULL? n->data : NULL);
}

// remove - determine which bucket; determine if the value
// we want to remove is in that bucket
void DeleteBE(BEHash* h, Hashkey key, BlockEntry* data) {
	int index = (h->key_hash)(key) % h->num_buckets;
	BEList *bucket = h->buckets[index];
	if (bucket != NULL) {
		removeIfExists(bucket, data);
	}
        h->num_entries--;
}

// Free all the memory used by the given BEHash
void deleteHash(BEHash **hashtable_ptr, deleter delete_fn) {
	//free the memory associated with hash
	if ((hashtable_ptr != NULL) && (*hashtable_ptr != NULL)) {
		BEHash *hash_table = *hashtable_ptr;
		for(int i = 0; i < hash_table->num_buckets; i++) {
			deleteBEList(&hash_table->buckets[i], delete_fn);
		}
		free(hash_table->buckets);
		free(hash_table);
		*hashtable_ptr = NULL;
	}
	
}
