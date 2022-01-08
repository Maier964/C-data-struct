#include "ccvector.h"
#include "common.h"
#include "string.h"
#include "stdio.h" // Delete this

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

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        /// Use an aux to store the vector pointer
        aux = realloc(Vector->Array, Vector->Count * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            printf("Realloc succesful");
        }
        else
        {
            return -1;
        }
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

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

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        /// Use an aux to store the vector pointer
        aux = realloc(Vector->Array, Vector->Count * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            printf("Realloc succesful");
        }
        else
        {
            return -1;

        }
    }
    for (int i = Vector->Count - 1; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }

    Vector->Array[0] = Value;
    Vector->Count++;

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
        aux = realloc(Vector->Array, Vector->Count * sizeof(int));
        if (NULL != aux)
        {
            Vector->Array = aux;
            printf("Realloc succesful");
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
    // Index vs position discussion. Index should start at 0 while position starts at 1? 

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

    if (NULL == Vector)
    {
        return -1; // Nothing to clear
    }


    memset(Vector->Array, 0, Vector->Count * sizeof(int));


    // Reset initial size
    Vector->Size = INITIAL_SIZE; // + Realloc 
    Vector->Count = 0;




    return 0;
}

int VecSort(CC_VECTOR* Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    // Implement QuickSort

    return -1;
}

int VecAppend(CC_VECTOR* DestVector, CC_VECTOR* SrcVector) // Needs to be tested
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
        return -1;
    }

    // SrcVector will be appended to DestVector
    // First parameter is the destination zone : A pointer to the end of the destination array
    // Second parameter is the source memory area, i.e. a pointer to the first element of the source array.
    // Third parameter is the number of bytes to be copied -> the number of ints in the source array 
    memcpy(DestVector->Array + sizeof(int) * DestVector->Count, SrcVector->Array, sizeof(int) * SrcVector->Count);


    return 0;
}