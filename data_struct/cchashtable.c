// Hopscotch Hashing
// Each bucket will be part of H neighbourhoods and will have a neighbourhood of H buckets
// H is 32 to be sufficient for collisions but still be fetched in a single cache line.
// For reference : https://programming.guide/hopscotch-hashing.html

#include <string.h>
#include "cchashtable.h"
#include "common.h"


#define INITIAL_SIZE ((int)9.8317e4)

#define H 32

int HashDefaultFunction( char* Key)
{
    if ( NULL == Key )
    {
        LOG_ERROR(" Key %s is null", Key);
    }

    int len = GetStringLen(Key);
    int pow = 1;
    int hash = 0;

    for ( int i = 0; i < len; i++ )
    {   // What happens if you want a bigger hash table? Ans : Change macro or allocate another function, not the default one
        hash = (hash + ( Key[i] - 'a' + 1 ) * pow) % INITIAL_SIZE;
        pow = (pow * 31) % INITIAL_SIZE;
    }

    return hash % INITIAL_SIZE;
}

int CustomSizeHashFunction( char* Key, int Size )
{
    if ( NULL == Key )
    {
        LOG_ERROR(" Key %s is null", Key);
    }

    int len = GetStringLen(Key);
    int pow = 1;
    int hash = 0;

    for ( int i = 0; i < len; i++ )
    {   
        hash = (hash + ( Key[i] - 'a' + 1 ) * pow) % Size;
        pow = (pow * 31) % Size;
    }

    return hash % Size;
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

    // Initialising everything to -1 is no longer required,
    // as we dont use that default value anywhere
    memset( hashTableTemplate->Buckets, -1, INITIAL_SIZE * sizeof(CC_HASH_ITEM) );
//    memset( hashTableTemplate->Buckets->Head, NULL, INITIAL_SIZE * sizeof(CC_HASH_ITEM) );

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


int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value, int OriginalIndex)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    int position;
    int size;
    int blankSpot;
    int swappedEmptyBucket;

    if ( NULL == HashTable || NULL == Key )
    {
        return -1;
    }

    // position is also the head bucket of key
    position = HashTable->HashFunction( Key );
    size = HashTable->Size;

    // Lookup 
    if ( !HtGetKeyValue( HashTable, Key, Value ) )
    {
        return -1;
    }

    // Position is empty in hashtable
    if ( HashTable->Buckets[ position ].Key == NULL )
    {
        HtAddKeyAux( HashTable, Value, Key, position );
    }
    else
    {
        // Check neighbourhood of head bucket and search for empty spots
        // Hopscotch hashing is cyclic ( hence we use the "% size")
        for( int i = position+1; i < position+ H; i++ )
        {
            // Found empty spot
            if ( HashTable->Buckets[i % size].Key == NULL )
            {
                HtAddKeyAux( HashTable, Value, Key , i );
                return 0;
            }
        }

        // Neighbourhood is full. Find next black bucket..
        for ( int i = 0; i < size; i++ )
        {
            if ( HashTable->Buckets[ (i + position + H) % size ].Key != NULL )
            {
                // Found blank spot.
                blankSpot = (i + position + H) % size;

                swappedEmptyBucket = HtInsertNeighbourhoodAux(
                    HashTable, position, blankSpot
                );

                HtAddKeyAux( HashTable, Value, Key, swappedEmptyBucket );

                return 0;
            }
        }

    }

    // unable to add.. this should be impossible if we implement the expand function
    return -1;
}

int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    int position;
    int size;

    if ( NULL == HashTable || NULL == Value )
    {
        return -1;
    }

    position = HashTable->HashFunction(Key);
    size = HashTable->Size;

    // Check for head bucket and its neighbourhood
    for ( int i = position; i <= position + H; i++ )
    {
        if ( HashTable->Buckets[i % size].Value == Value 
            && CustomStrCmp( HashTable->Buckets[i % size].Key, Key ) )
            {
                // Found key
                return 0;
            }
        else goto exit;
    }

    exit:

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

// Dunno if ill ever use this funtion tbh
int HtAddKeyAux( PCC_HASH_TABLE HashTable, int Value, char* Key, int Pos )
{

    if ( NULL == HashTable || NULL == Pos )
    {
        return -1;
    }

    HashTable->Buckets[Pos].Value = Value;
    HashTable->Buckets[Pos].Value = (char*)malloc( sizeof( char ) * GetStringLen(Key) );
    HashTable->Buckets[Pos].Key = Key;

    return 0;
}


int HtInsertNeighbourhoodAux( PCC_HASH_TABLE HashTable, int headBucket, int blankSpot )
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(headBucket);
    CC_UNREFERENCED_PARAMETER(blankSpot);

    int candidateBucket;
    int size;
    int candidateHeadBucket;

    if ( NULL == HashTable )
    {
        LOG_ERROR(" Hash table at %p is NULL ", HashTable);
        return -1;
    }

    size = HashTable->Size;

    // While blankSpot and head bucket are in different neighbourhoods
    while ( headBucket >= blankSpot && headBucket < ( blankSpot + H ) )
    {
        // Look for swaps
        for ( int i = 0; i < H; i++ )
        {
            // Check if blankSpot can be inserted higher in its neighbourhood.
            // Also keep note of cyclic nature of the hash table
            // Example : blankSpot - H + 0 tests the first bucket of blankSpot's neighbourhood
            candidateBucket = ( size + blankSpot - H + i ) % size;

            // Find head bucket of candidateBucket... 
            candidateBucket = HashTable->HashFunction( 
                HashTable->Buckets[ candidateBucket ].Key );

            // Check if candidateHeadBucket is in the same neighbourhood as blankSpot
            // Same condition as line 330 but reversed
            if ( candidateHeadBucket < blankSpot && candidateHeadBucket > ( blankSpot + H ))
            {
                // Candidate bucket is good! Proceed to swap values.
                // blankSpot doesn't contain data so no need for auxiliary variable
                HashTable->Buckets[ blankSpot ].Key = HashTable->Buckets[ candidateBucket ].Key;
                
                HashTable->Buckets[ blankSpot ].Value = HashTable->Buckets[ candidateBucket ].Value;

                HashTable->Buckets[ candidateBucket ].Key = NULL;

                HashTable->Buckets[ candidateBucket ].Value = -1;

                return candidateBucket;
            }

        }

        LOG_ERROR( "Hash table at is full. Size is %d Expanding.. ", HashTable->Size );
        // EXPAND HashTable... Imd, nu mai am chef acum..
    }
}