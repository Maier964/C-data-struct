// Hopscotch Hashing
// Each bucket will be part of H neighbourhoods and will have a neighbourhood of H buckets
// H is 64 to be sufficient for collisions but still be fetched in a single cache line.
// For reference : https://programming.guide/hopscotch-hashing.html

#include "cchashtable.h"
#include "common.h"
#include <string.h>


#define INITIAL_SIZE ((int)9.8317e4)

#define H 64

int expansionPrimeNrs[2] = { (int)1.96613e5 , (int)3.93241e5 }; 

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
    {   
        hash = (hash + ( Key[i] - 'a' + 1 ) * pow) % INITIAL_SIZE;
        pow = (pow * 31) % INITIAL_SIZE;
    }


    return (hash > 0) ? (hash % INITIAL_SIZE) : (-1 * hash) % INITIAL_SIZE;
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


    for (int i = 0; i < INITIAL_SIZE; i++)
    {
        hashTableTemplate->Buckets[i].Value = -1;
        hashTableTemplate->Buckets[i].Key = NULL;
    }

    hashTableTemplate->Count = 0;

    hashTableTemplate->Size = INITIAL_SIZE;

    hashTableTemplate->HashFunction = HashDefaultFunction;

    hashTableTemplate->TimesExpanded = 0;

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

    HtClear( *HashTable );

    free((*HashTable)->Buckets);

    HashTable = NULL;

    return 0;
}


int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value)
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
    if ( !HtGetKeyValue( HashTable, Key, &Value ) )
    {
        return -1;
    }

    // Check if hash table needs expansion.
    if ( (float)HashTable->Count / HashTable->Size > 0.7 )
    {
        // New size can be a new prime that is 2 times greater than
        // the original size ( minimum )
        HtExpand( HashTable, expansionPrimeNrs[ HashTable->TimesExpanded ] );
    }

    // Position is empty in hashtable
    if ( HashTable->Buckets[ position ].Key == NULL )
    {
        HtAddKeyAux( HashTable, Value, Key, position );
        return 0;
    }
    else
    {
        // Check neighbourhood of head bucket and search for empty spots
        // Hopscotch hashing is cyclic ( hence we use the "% size")
        for( int i = position+1; i < position+ H; i++ )
        {
            
            if ( HashTable->Buckets[i % size].Key == NULL )
            {
                // Found empty spot
                HtAddKeyAux( HashTable, Value, Key , i );
                return 0;
            }
            
            if ( !CustomStrCmp( HashTable->Buckets[ i % size ].Key, Key ) )
            {
                // Found key.. No duplicates allowed
                LOG_ERROR(" Key %s is already in hash table.", Key);
                return -1;
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


    if (NULL == HashTable)
    {
        return -1;
    }

    position = HashTable->HashFunction(Key);
    size = HashTable->Size;

    // Check for head bucket and its neighbourhood
    for ( int i = position; i <= position + H; i++ )
    {
        if (HashTable->Buckets[i % size].Key != NULL)
        {
            if (!(CustomStrCmp(HashTable->Buckets[i % size].Key, Key)))
            {
                // Found key
                *Value = HashTable->Buckets[i % size].Value;
                return 0;
            }
        }

    }

    return -1;
}

int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    int headBucket;
    int size;

    if ( NULL == HashTable || NULL == Key )
    {
        return -1;
    }

    headBucket = HashTable->HashFunction( Key );
    size = HashTable->Size;

    for ( int i = headBucket; i < H + headBucket; i++)
    {
        if ( !CustomStrCmp( HashTable->Buckets[ i % size ].Key, Key ) )
        {
            // Found key, removing
            HashTable->Buckets[i % size].Key = NULL;
            HashTable->Buckets[ i % size ].Value = -1;
            HashTable->Count--;
            return 0;
        }
    }

    // Key not found
    return -1;
}

int HtHasKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    int headBucket;
    int size;

    if ( NULL == HashTable || NULL == Key )
    {
        return -1;
    }

    headBucket = HashTable->HashFunction( Key );
    size = HashTable->Size;

    for ( int i = headBucket; i < H + headBucket; i++)
    {
        if ( !CustomStrCmp( HashTable->Buckets[ i % size ].Key, Key ) )
        {
            // Key found
            return 1;
        }
    }

    // Key not found
    return 0;
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
    iterator->CurrentKey = NULL;
    iterator->CurrentCount = 0;
    iterator->CurrentIndex = 0;

    *Iterator = iterator;

    if ( HashTable->Count == 0 )
    {
        // No keys in hash table
        return -2;
    }

    for( int i = 0; i < HashTable->Size; i++ )
    {
        if ( HashTable->Buckets[i].Key != NULL )
        {
            // First value found
            (*Iterator)->CurrentKey = &(HashTable->Buckets[i]);
            (*Iterator)->CurrentCount = 1;
            (*Iterator)->CurrentIndex = i;
        }
    }

    // Try to find a more efficient way to do this.. 
    // Iterating trough all buckets is not adequate.

    // No keys in hash table.
    return -2;
}


int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key)
{
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Iterator);

    if ( NULL == Iterator || NULL == Key )
    {
        return -1;
    }

    if ( Iterator->CurrentCount == Iterator->HashTable->Count )
    {
        return -2;
    }


    for ( int i = Iterator->CurrentIndex; i < Iterator->HashTable->Count; i++ )
    {
        if ( Iterator->HashTable->Buckets[i].Key != NULL )
        {
            // Next value found
            Iterator->CurrentKey = &(Iterator->HashTable->Buckets[i]);
            Iterator->CurrentCount++;
            Iterator->CurrentIndex = i;
        }
    }

    return -2;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator)
{
    CC_UNREFERENCED_PARAMETER(Iterator);

    if ( NULL == Iterator )
    {
        return -1;
    }

    free( (*Iterator)->CurrentKey );
    free( *Iterator );

    Iterator = NULL;

    return 0;
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

int HtAddKeyAux( PCC_HASH_TABLE HashTable, int Value, char* Key, int Pos )
{

    if ( NULL == HashTable )
    {
        return -1;
    }

    HashTable->Buckets[Pos].Value = Value;
    HashTable->Buckets[Pos].Key = (char*)malloc( sizeof( char ) * GetStringLen(Key) );
    HashTable->Buckets[Pos].Key = Key;
    HashTable->Count++;

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
            candidateHeadBucket = HashTable->HashFunction( 
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

        LOG_ERROR( "Hash table at is full. Size is %d. Expanding.. ", HashTable->Size );
        
        HtExpand( HashTable, expansionPrimeNrs[ HashTable->TimesExpanded ] );

        return 0;
    }

    return 0;
}


int HtExpand( PCC_HASH_TABLE HashTable, int newSize )
{
    PCC_HASH_ITEM hashTblAux = NULL;

    if ( NULL == HashTable || newSize < HashTable->Size )
    {
        return -1;
    }

    hashTblAux = realloc( HashTable->Buckets, newSize * sizeof( CC_HASH_ITEM ) );

    if ( NULL == hashTblAux )
    {
        LOG_ERROR("Hash table at expansion failed at %p", HashTable);
        return -1;
    }

    // Should have removed every item from the first hash and introduced it in a second hash... :c

    HashTable->Buckets = hashTblAux;

    HashTable->Size = newSize;

    HashTable->TimesExpanded++;

    return 0;
}

void HtPrint( PCC_HASH_TABLE HashTable )
{
    for( int i = 0; i < HashTable->Size; i++ )
    {
        if ( HashTable->Buckets[i].Key != NULL )
        {
            printf(" Bucket %d : Key %s : Value %d \n ", i , HashTable->Buckets[i].Key, HashTable->Buckets[i].Value);
        }
    }
}