#include "ccheap.h"
#include "common.h"


int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    CC_UNREFERENCED_PARAMETER(MaxHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    if ( NULL == MaxHeap )
    {
        return -1;
    }

    *MaxHeap = ( PCC_HEAP ) malloc( sizeof( CC_HEAP ) );

    (*MaxHeap)->Type = MAXHEAP;

    if ( NULL != InitialElements )
    {

        VecCreate(&(*MaxHeap)->Elements);
        
        // Assure that if the InitialElements vector is deleted, the heap will still function properly
        CustomMemcpy( (*MaxHeap)->Elements->Array, InitialElements->Array, InitialElements->Count);
        (*MaxHeap)->Elements->Count = InitialElements->Count;
    }
    else
    {
        // Create a vector with default configuration ( size of 100 could waste memory but we have no knowlegde of input dimenstion )
        VecCreate( &((*MaxHeap)->Elements) );
    }

    // Convert array to heap

    for ( int i = ((*MaxHeap)->Elements->Count / 2) - 1; i >= 0 ; i-- )
    {
        Heapify( *MaxHeap, i );
    }
    

    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    CC_UNREFERENCED_PARAMETER(MinHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    // Same thought process as in the HpCreateMaxHeap function

    if ( NULL == MinHeap )
    {
        return -1;
    }

    *MinHeap = ( PCC_HEAP ) malloc( sizeof( CC_HEAP ) );

    (*MinHeap)->Type = MINHEAP;

    if ( NULL != InitialElements )
    {
        VecCreate(&(*MinHeap)->Elements);

        CustomMemcpy( (*MinHeap)->Elements->Array, InitialElements->Array, InitialElements->Count );

        (*MinHeap)->Elements->Count = InitialElements->Count;
    } 
    else
    {
        
        VecCreate(&(*MinHeap)->Elements);
    }


    // Create MinHeap

    for ( int i = ((*MinHeap)->Elements->Count / 2) - 1; i >= 0 ; i-- )
    {
        Heapify( *MinHeap, i );
    }

    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
    CC_UNREFERENCED_PARAMETER(Heap);

    if ( NULL == Heap )
    {
        return -1;
    }

    // Delete the array first
    VecDestroy( &((*Heap)->Elements) );

    free( *Heap );

    Heap = NULL;

    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Heap )
    {
        return -1;
    }

    VecInsertTail( Heap->Elements, Value );

    Swim( Heap, Heap->Elements->Count - 1 );

    return 0;
}

// Remove all elements with value "Value"
int HpRemove(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Heap )
    {
        return -1;
    }

    // This could be improved, its a bit ugly since Count is dynamically modified in the for loop
    for( int i = 0; i < Heap->Elements->Count; i++ )
    {
        if ( Heap->Elements->Array[i] == Value )
        {
            // Remove item from heap
            Swap( &Heap->Elements->Array[i] , 
            &Heap->Elements->Array[ Heap->Elements->Count - 1 ] );

            VecRemoveByIndex( Heap->Elements, Heap->Elements->Count - 1 );

            Heapify( Heap, i );
        }
    }

    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);

    if ( NULL == Heap || NULL == ExtremeValue )
    {
        return -1;
    }   

    if( Heap->Elements->Count != 0 )
    {
        *ExtremeValue = Heap->Elements->Array[0];
    }
    else
    {
        LOG_ERROR("Heap at %p is empty", Heap);
    }
    
    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);

    if ( HpGetExtreme( Heap, ExtremeValue ) )
    {
        return -1;
    }

    HpRemove( Heap, *ExtremeValue );

    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    CC_UNREFERENCED_PARAMETER(Heap);

    if ( NULL == Heap )
    {
        return -1;
    }

    return Heap->Elements->Count;    
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(SortedVector);

    int* reallocVec = NULL;

    // Assume SortedVector is initialised
    if ( NULL == Heap || SortedVector == NULL )
    {
        return -1;
    }

    // Count - 1 = Last element
    int reverseIterator = Heap->Elements->Count - 1;
    // Optional sanitisation
    VecClear(SortedVector);

    if ( SortedVector->Size >= Heap->Elements->Count )
    {
        SortedVector->Count = Heap->Elements->Count;
    }
    else
    // Make sure there's enough space in the vector ( Realloc )
    {
        reallocVec = realloc( SortedVector->Array, Heap->Elements->Count * sizeof(int) );

        if ( reallocVec == NULL )
        {
            return -1;
        }

        SortedVector->Array = reallocVec;
    }
    

    // Simple HeapSort
    if( Heap->Type == MAXHEAP )
    {
        // Elements are still present in heap
        while( reverseIterator >= 0 )
        {
            // Get max element
            Swap( &Heap->Elements->Array[0], &Heap->Elements->Array[ 
                reverseIterator] );

            SortedVector->Array[ reverseIterator ] =
            Heap->Elements->Array [ reverseIterator ];

            Heap->Elements->Count--;    reverseIterator--;

            Heapify( Heap, 0 );

            
        }
    }
    else
    {
        while( Heap->Elements->Count > 0 )
        {
            Swap( &Heap->Elements->Array[0], &Heap->Elements->Array[
                Heap->Elements->Count-1] );

            SortedVector->Array[ reverseIterator - Heap->Elements->Count + 1 ] =
            Heap->Elements->Array[ Heap->Elements->Count - 1 ];

            Heap->Elements->Count--;

            Heapify( Heap, 0 );
        }
    }

    // Restore acces to intial heap elements.
    Heap->Elements->Count = SortedVector->Count;

    return 0;
}


int Heapify( PCC_HEAP Heap, int Index )
{

    if ( NULL == Heap )
    {
        return -1;
    }

    int count = Heap->Elements->Count - 1;

    // Determine type
    if ( Heap->Type == MAXHEAP )
    {
        // Maxheap
        while( (LEFT( Index ) < count && LEFT_ELEMENT( Index ) > Heap->Elements->Array[ Index ] )|| 
         (RIGHT(Index) < count && RIGHT_ELEMENT(Index) > Heap->Elements->Array[ Index ] ) )
        {
            if ( LEFT_ELEMENT(Index) > Heap->Elements->Array[ Index ] && 
            LEFT_ELEMENT(Index) >= RIGHT_ELEMENT(Index) )
            {
                Swap(&LEFT_ELEMENT(Index), &Heap->Elements->Array[Index]);
                Index = LEFT( Index );
            }
            else
            {
                Swap( &RIGHT_ELEMENT(Index), &Heap->Elements->Array[Index] );
                Index = RIGHT( Index );
            }
        }
    }
    else
    {
        // Minheap
        while( (LEFT( Index ) < count && LEFT_ELEMENT( Index ) < Heap->Elements->Array[ Index ] )|| 
         (RIGHT(Index) < count && RIGHT_ELEMENT(Index) < Heap->Elements->Array[ Index ] ) )
        {
            if ( LEFT_ELEMENT(Index) < Heap->Elements->Array[ Index ] && 
            LEFT_ELEMENT(Index) <= RIGHT_ELEMENT(Index) )
            {
                Swap(&LEFT_ELEMENT(Index), &Heap->Elements->Array[Index]);
                Index = LEFT( Index );
            }
            else
            {
                Swap( &RIGHT_ELEMENT(Index), &Heap->Elements->Array[Index] );
                Index = RIGHT( Index );
            }
        }
        

    }

    
    return 0;
}


int Swim( PCC_HEAP Heap, int Index )
{

    if ( NULL == Heap )
    {
        return -1;
    }

    if ( Heap->Type == MAXHEAP )
    {
        while( Heap->Elements->Array[ Index ] > PARENT_ELEMENT(Index) && PARENT( Index ) >= 0 )
        {
            Swap( &Heap->Elements->Array[ Index ], &PARENT_ELEMENT(Index) );
            Index = PARENT(Index);
        }
    }
    else 
    {
        while( Heap->Elements->Array[ Index ] < PARENT_ELEMENT(Index) && PARENT( Index ) >= 0 )
        {
            Swap( &Heap->Elements->Array[ Index ], &PARENT_ELEMENT(Index) );
            Index = PARENT(Index);
        }
    }


    return 0;
}