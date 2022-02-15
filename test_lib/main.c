#include <stdio.h>
#include "ccvector.h"
#include "ccstack.h"
#include "cchashtable.h"
#include "ccheap.h"
#include "cctree.h"
#include "common.h"
#include "stdint.h"

int TestVector();
int TestStack();
int TestHashTable();
int TestHeap();
int TestTree();


// Custom tests
int CustomVectorTest();

void RunTests();


int main(void)
{
    //RunTests();
    
    //TestVector();

//    CustomVectorTest();

    char* alpha = "abcdefghijklmnopqwe123SD";

    for (int i = 0; i < 24; i++)
        printf("%d ", HashDefaultFunction(alpha[i]));

     return 0;
}

void RunTests()
{
    /// NOTE: The tests provided here are by no means exhaustive and are only
    /// provided as a starting point (not all functions are tested, not all use cases
    /// and failure scenarios are covered). You are encouraged to expand these tests
    /// to include missing scenarios.
    if (0 == TestVector())
    {
        printf("Vector test passed\n\n");
    }
    else
    {
        printf("Vector test failed\n\n");
    }

    if (0 == TestStack())
    {
        printf("Stack test passed\n\n");
    }
    else
    {
        printf("Stack test failed\n\n");
    }

    if (0 == TestHashTable())
    {
        printf("HashTable test passed\n\n");
    }
    else
    {
        printf("HashTable test failed\n\n");
    }

    if (0 == TestHeap())
    {
        printf("Heap test passed\n\n");
    }
    else
    {
        printf("Heap test failed\n\n");
    }

    if (0 == TestTree())
    {
        printf("Tree test passed\n\n");
    }
    else
    {
        printf("Tree test failed\n\n");
    }
}


int TestTree()
{
    int retVal = -1;
    CC_TREE* usedTree = NULL;

    retVal = TreeCreate(&usedTree);
    if (0 != retVal)
    {
        printf("TreeCreate failed!\n");
        goto cleanup;
    }

    retVal = TreeInsert(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeInsert failed!\n");
        goto cleanup;
    }

    if (1 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = TreeRemove(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeRemove failed!\n");
        goto cleanup;
    }

    if (0 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value after remove!\n");
        retVal = -1;
        goto cleanup;
    }

    if (0 != TreeGetCount(usedTree))
    {
        printf("TreeGetCount invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedTree)
    {
        if (0 != TreeDestroy(&usedTree))
        {
            printf("TreeDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}
              
int TestHeap()
{
    int retVal = -1;
    int foundVal = -1;
    CC_HEAP* usedHeap = NULL;

    retVal = HpCreateMinHeap(&usedHeap, NULL);
    if (0 != retVal)
    {
        printf("HpCreateMinHeap failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 20);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 10);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    if (2 != HpGetElementCount(usedHeap))
    {
        printf("Invalid element count!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = HpGetExtreme(usedHeap, &foundVal);
    if (0 != retVal)
    {
        printf("HpGetExtreme failed!\n");
        goto cleanup;
    }

    if (10 != foundVal)
    {
        printf("Invalid minimum value returned!");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedHeap)
    {
        if (0 != HpDestroy(&usedHeap))
        {
            printf("HpDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}

int TestHashTable()
{
    int retVal = -1;
    int foundVal = -1;
    CC_HASH_TABLE* usedTable = NULL;

    retVal = HtCreate(&usedTable);
    if (0 != retVal)
    {
        printf("HtCreate failed!\n");
        goto cleanup;
    }

    retVal = HtSetKeyValue(usedTable, "mere", 20);
    if (0 != retVal)
    {
        printf("HtSetKeyValue failed!\n");
        goto cleanup;
    }

    if (1 != HtHasKey(usedTable, "mere"))
    {
        printf("Invalid answer to HtHasKey!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = HtGetKeyValue(usedTable, "mere", &foundVal);
    if (0 != retVal)
    {
        printf("HtGetKeyValue failed!\n");
        goto cleanup;
    }

    if (foundVal != 20)
    {
        printf("Invalid value after get!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedTable)
    {
        if (0 != HtDestroy(&usedTable))
        {
            printf("HtDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}

int TestStack()
{
    int retVal = -1;
    int foundVal = -1;
    CC_STACK* usedStack = NULL;


    PCC_STACK stack2 = NULL;
    StCreate(&stack2);

    retVal = StCreate(&usedStack);
    if (0 != retVal)
    {
        printf("StCreate failed!\n");
        goto cleanup;
    }

    retVal = StPush(usedStack, 10);
    if (0 != retVal)
    {
        printf("StPush failed!\n");
        goto cleanup;
    }

    StPrint(usedStack);

    // Don't test this since return values are contradictory
    
    //if (1 != StIsEmpty(usedStack))
    //{
    //    printf("Invalid answer to StIsEmpty!\n");
    //    retVal = -1;
    //    goto cleanup;
    //}



    StPrint(usedStack);

    retVal = StPop(usedStack, &foundVal);
    if (0 != retVal)
    {
        printf("StPop failed!\n");
        goto cleanup;
    }

    StPrint(usedStack);

    if (foundVal != 10)
    {
        printf("Invalid value after pop!\n");
        retVal = -1;
        goto cleanup;
    }

    printf("Custom test for append:\n");



    StPush( usedStack, 3 );
    StPush( usedStack, 2 );
    StPush( usedStack, 1 );

    StPush( stack2, 5 );
    StPush( stack2, 4 );
    StPush( stack2, 1 );

    

    StPrint(usedStack);
    StPrint(stack2);


    StPushStack( usedStack, stack2 );

    StPrint(usedStack);
    StPrint(stack2);


cleanup:
    if (NULL != usedStack)
    {
        if (0 != StDestroy(&usedStack))
        {
            printf("StDestroy failed on usedStack!\n");
            retVal = -1;
        }
    }

    if ( NULL != stack2 )
    {
        if ( 0 != StDestroy(&stack2) )
        {
            printf("StDestroy failed on stack2!\n");
            retVal = -1;
        }
    }

    return retVal;
}

int TestVector()
{
    int retVal = -1;
    int foundVal = 0;
    CC_VECTOR* usedVector = NULL;
    
    retVal = VecCreate(&usedVector);
    if (0 != retVal)
    {
        printf("VecCreate failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecInsertTail(usedVector, 10);
    if (0 != retVal)
    {
        printf("VecInsertTail failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecInsertHead(usedVector, 16);
    if (0 != retVal)
    {
        printf("VecInsertHead failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    if (VecGetCount(usedVector) != 2)
    {
        printf("Invalid count returned!\n");
        retVal = -1;
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecInsertAfterIndex(usedVector, 0, 20);
    if (0 != retVal)
    {
        printf("VecInsertAfterIndex failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecRemoveByIndex(usedVector, 0);
    if (0 != retVal)
    {
        printf("VecRemoveByIndex failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecGetValueByIndex(usedVector, 0, &foundVal);
    if (0 != retVal)
    {
        printf("VecGetValueByIndex failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    if (foundVal != 20)
    {
        printf("Invalid value found at position 0\n");
        retVal = -1;
        goto cleanup;
    }

    VecPrint(usedVector);

    retVal = VecClear(usedVector);
    if (0 != retVal)
    {
        printf("VecClear failed!\n");
        goto cleanup;
    }

    VecPrint(usedVector);

    if (0 != VecGetCount(usedVector))
    {
        printf("Invalid count after clear\n");
        retVal = -1;
        goto cleanup;
    }

    VecPrint(usedVector);

cleanup:
    if (NULL != usedVector)
    {
        if (0 != VecDestroy(&usedVector))
        {
            printf("VecDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}


int CustomVectorTest()
{
    CC_VECTOR* test = NULL;

    VecCreate(&test);


    VecInsertHead(test, 6);

    VecInsertHead(test, 50);

    VecInsertHead(test, 21);

    VecInsertHead(test, 100);

    VecInsertHead(test, 82);

    VecInsertHead(test, 8);

    VecInsertHead(test, 2);


    VecPrint(test);

    VecSort(test);

    VecPrint(test);


    VecDestroy(&test);

    return 0;
}