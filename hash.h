/* 
 * File:   hash.h
 * Author: colm_mchugh
 *
 * Created on July 25, 2017, 5:54 PM
 */

#ifndef HASH_H
#define	HASH_H

#include "hash_defs.h"

unsigned long hashDjb2(Hashkey key);

unsigned long sdbm(Hashkey key);

unsigned long jenkins1AtTime(Hashkey key);

unsigned long rolHash(Hashkey key);

unsigned long sdgwckHash(Hashkey key);


#endif	/* HASH_H */

