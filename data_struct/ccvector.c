#include "ccvector.h"
#include "common.h"
#include <string.h>

#define INITIAL_SIZE    100

int VecCreate(CC_VECTOR** Vector)
{
    CC_VECTOR* vec = NULL;

    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));

    if (NULL == vec)
    {
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc(sizeof(int) * INITIAL_SIZE);

    if (NULL == vec->Array)
    {
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}

int VecDestroy(CC_VECTOR** Vector)
{
    CC_VECTOR* vec = *Vector;

    if (NULL == Vector)
    {
        return -1;
    }

    free(vec->Array);
    vec->Array = NULL;

    free(vec);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR* Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    int* aux = NULL;

    if (NULL == Vector)
    {
        return -1;
    }

    // If vector memory is maxed
    if (Vector->Count + 1 == Vector->Size)
    {
        /// REALLOC
        /// Allow an additonal (10% * size) slots every time array exceeds size.
        aux = realloc(Vector->Array, (Vector->Size + (10* Vector->Size ) / 100) * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            Vector->Size += (10 * Vector->Size) / 100;
        }
        else
        {
            return -1;
        }
    }

    Vector->Array[Vector->Count++] = Value;

    return 0;
}

int VecInsertHead(CC_VECTOR* Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    int* aux = NULL;

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count + 1 >= Vector->Size)
    {
        /// REALLOC 
        aux = realloc(Vector->Array, (Vector->Size+ (10* Vector->Size ) / 100) * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            Vector->Size+=((10* Vector->Size ) / 100);
        }
        else
        {
            printf("Realloc failed.\n");
            return -1;
        }
    }

    for (int i = ++Vector->Count; i > 0; i--)
    {
        Vector->Array[i] = Vector->Array[i-1];
    }

    Vector->Array[0] = Value;

    return 0;
}

int VecInsertAfterIndex(CC_VECTOR* Vector, int Index, int Value) // This function inserts at index
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    int* aux = NULL;

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        /// Use an aux to store the vector pointer
        aux = realloc(Vector->Array + 1, Vector->Count * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            printf("Realloc succesful");
            Vector->Size++;
        }
        else
        {
            return -1;
        }
    }


    for (int i = Vector->Count - 1; i >= Index + 1; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }

    Vector->Array[Index + 1] = Value;
    Vector->Count++;

    return 0;
}

int VecRemoveByIndex(CC_VECTOR* Vector, int Index)
{ 

    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);

    if (NULL == Vector)
    {
        return -1;
    }

    if (0 > Index)
    {
        return -1; // Index cannot be negative
    }

    if (Vector->Count < Index)
    {
        return -1; // Index is out of bounds
    }

    for (int i = Index; i < Vector->Count; i++) // Considering index starts at 0.
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }

    Vector->Count--;

    return 0;
}

int VecGetValueByIndex(CC_VECTOR* Vector, int Index, int* Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);


    if (NULL == Vector)
    {
        return -1;
    }

    // Index out of bounds
    if ( Vector->Count < Index )
    {
        return -1;
    }

    *Value = Vector->Array[Index];


    return 0;
}

int VecGetCount(CC_VECTOR* Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR* Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    int* auxRealloc = NULL;

    if (NULL == Vector)
    {
        // Nothing to clear
        return -1; 
    }

    CustomIntMemSet(Vector->Array, 0, Vector->Count );


    // Reset initial size
    Vector->Size = INITIAL_SIZE; 

    // Realloc
    auxRealloc = realloc( Vector->Array, INITIAL_SIZE * sizeof(int) );

    if ( auxRealloc == NULL )
    {
        LOG_ERROR("Realloc failed at %p", auxRealloc);
        return -1;
    }

    Vector->Array = auxRealloc;

    Vector->Count = 0;


    return 0;
}

int VecSort(CC_VECTOR* Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    QuickSort(Vector->Array, 0, Vector->Count - 1);

    return 0;
}

int VecAppend(CC_VECTOR* DestVector, CC_VECTOR* SrcVector) 
{
    CC_UNREFERENCED_PARAMETER(DestVector);
    CC_UNREFERENCED_PARAMETER(SrcVector);

    int* aux = NULL;

    if (NULL == DestVector || NULL == SrcVector)
    {
        return -1;
    }

    // Realloc DestVector
    aux = realloc(DestVector->Array, sizeof(int) * (SrcVector->Size + DestVector->Size));

    // Realloc error check
    if (NULL == aux)
    {
        DestVector->Array;
        return -1;
    }


    // SrcVector will be appended to DestVector
    for (int i = DestVector->Count; i < DestVector->Count + SrcVector->Count; i++) 
    {
        DestVector->Array[i] = SrcVector->Array[i - DestVector->Count];
    }
    

    // Update count idx
    DestVector->Count += SrcVector->Count;


    return 0;
}

int VecPrint(CC_VECTOR* Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    
    if (NULL == Vector)
    {
        return -1;
    }

    printf("Vector is: ");

    if (!Vector->Count)
    {
        printf("Empty vector.");
    }

    for (int i = 0; i < Vector->Count; i++)
    {
        printf("%d ", Vector->Array[i]);
    }

    putchar(10); // Put a newline after printing

    return 0;
}