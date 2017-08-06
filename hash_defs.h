/* 
 * File:   hash_defs.h
 * Author: colm_mchugh
 *
 * Created on August 5, 2017, 2:27 PM
 */

#ifndef HASH_DEFS_H
#define	HASH_DEFS_H

#define DEFAULT_NUM_BUCKETS 4783

typedef const char*  Hashkey;

typedef bool (*f_compare)(Hashkey, Hashkey);

typedef unsigned long (*f_hash)(Hashkey);

#endif	/* HASH_DEFS_H */

