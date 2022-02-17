#include <string.h>
#include "cchashtable.h"
#include "common.h"


#define INITIAL_SIZE (int)1e+8

// Hopscotch Hashing using cache-friendly neighbourhood bucket fetching
#define H 32

int HashDefaultFunction( char* Key )
{
    int val = 0;
    
    uint32_t desired = (int32_t)Key;

    do{
        desired ^= desired >> 7;
        desired ^= desired << 9;
        desired ^= desired >> 13;
        val++;
    }while( desired != ((int32_t)Key ^ 0xAB1Cu) );

    return val % INITIAL_SIZE;
}

int HtCreate(CC_HASH_TABLE **HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);

    PCC_HASH_TABLE hashTableTemplate = NULL;

    if ( HashTable == NULL )
    {
        return -1;
    }

    hashTableTemplate = (PCC_HASH_TABLE)malloc(sizeof(CC_HASH_TABLE));

    hashTableTemplate->Buckets = (CC_HASH_ITEM*)malloc(INITIAL_SIZE * sizeof(CC_HASH_ITEM));

    // Hope this works
    memset( hashTableTemplate->Buckets, 0, INITIAL_SIZE * sizeof(CC_HASH_ITEM) );

    hashTableTemplate->Count = 0;

    hashTableTemplate->Size = INITIAL_SIZE;

    hashTableTemplate->HashFunction = HashDefaultFunction;

    *HashTable = hashTableTemplate;

    return 0;
}

int HtDestroy(CC_HASH_TABLE **HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);

    if ( NULL == HashTable )
    {
        return -1;
    }

    PCC_HASH_TABLE temp = *HashTable;

    free(temp->Buckets);
 //   free(temp->HashFunction);

    free(temp);

    return 0;
}

int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == HashTable || NULL == Key )
    {
        return -1;
    }

    // Position is empty in hashtable
    if ( HashTable->Buckets[ HashTable->HashFunction( HashTable->Buckets->Key ) ].Key )
    {

    }

    return 0;
}

// Lookup is a bit tricky, I didn't quite get it
int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);
    return -1;
}

int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    return -1;
}

int HtHasKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    return -1;
}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR **Iterator, char **Key)
{
    CC_HASH_TABLE_ITERATOR *iterator = NULL;

    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
    if (NULL == iterator)
    {
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    iterator->HashTable = HashTable;
    // INITIALIZE THE REST OF THE FIELDS OF iterator

    *Iterator = iterator;

    // FIND THE FIRST KEY AND SET Key

    return 0;
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key)
{
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Iterator);
    return -1;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator)
{
    CC_UNREFERENCED_PARAMETER(Iterator);
    return -1;
}

int HtClear(CC_HASH_TABLE *HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);

    if ( NULL == HashTable )
    {
        return -1;
    }

    free(HashTable->Buckets);

    HashTable->Buckets = (CC_HASH_ITEM*)malloc( INITIAL_SIZE * sizeof(CC_HASH_ITEM) );

    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE *HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);

    if ( NULL == HashTable )
    {
        return -1;
    }

    return HashTable->Count;
}