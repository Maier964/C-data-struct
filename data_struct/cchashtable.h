#pragma once
#include "stdint.h"
#include "cclist.h"

typedef int (*PCC_HASH_TABLE_FUNCTION)( char* );

typedef struct _CC_HASH_ITEM{
    int Value;
    char* Key; 
} CC_HASH_ITEM, *PCC_HASH_ITEM;

typedef struct _CC_HASH_TABLE {
    CC_HASH_ITEM* Buckets;
    int Count;
    int Size;
    PCC_HASH_TABLE_FUNCTION HashFunction;
} CC_HASH_TABLE, *PCC_HASH_TABLE;

typedef struct _CC_HASH_TABLE_ITERATOR
{
    CC_HASH_TABLE *HashTable; // set by call to HtGetFirstKey
} CC_HASH_TABLE_ITERATOR;


int HtCreate(CC_HASH_TABLE **HashTable);
int HtDestroy(CC_HASH_TABLE **HashTable);

// Returns -1 if Key already exist in HashTable or the parameters are invalid
int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value);

// Returns -1 if Key does not exist in HashTable or the parameters are invalid
int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value);

// Returns -1 if Key does not exist in HashTable or the parameters are invalid
int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key);

//  Returns:
//       1  - HashTable contains Key
//       0  - HashTable does not contain Key
//      -1  - Error or invalid parameter
int HtHasKey(CC_HASH_TABLE *HashTable, char *Key);

// Initializes the iterator and gets the first key in the hash table
// Returns:
//       -1 - Error or invalid parameter
//       -2 - No keys in the hash table
//      >=0 - Success
int HtGetFirstKey(CC_HASH_TABLE *HashTable, CC_HASH_TABLE_ITERATOR **Iterator, char **Key);

// Returns the next key in the hash table contained in the iterator
// Iterator saves the state of the iteration
// Returns:
//       -1 - Error or invalid parameter
//       -2 - No more keys in the hash table
//      >=0 - Success
int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key);

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator);

// Removes every element in the hash table
int HtClear(CC_HASH_TABLE *HashTable);

// Returns the number of keys in the HashTable, or -1 in case of error
int HtGetKeyCount(CC_HASH_TABLE *HashTable);


int HashDefaultFunction( char* Key );


int HtAddKeyAux( PCC_HASH_TABLE, int, char*, int )
