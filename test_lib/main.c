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
int CustomTreeTest();
int CustomHeapTest();
int CustomHashTableTest();

void RunTests();


int main(void)
{
    //RunTests();
    
    //TestVector();

    CustomVectorTest();

    

//    for (int i = 0; i < 24; i++)
//        printf("%d ", HashDefaultFunction(alpha[i]));

//    CustomHashTableTest();

//    printf(" %d ", Max(12, 14));

//    CustomHeapTest();

//    TestHashTable();

//    CustomTreeTest();




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

    // Wrong
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


int CustomTreeTest()
{
    CC_TREE* usedTree = NULL;

    int aux[] = { 58225,57865,50376,38313,48595,23819,63552,25385,16708,29762,65052,67214,32202,7167,30078,29826,13521,68118
        ,62140,48950,31616,10663,44270,48063,9734,22246,65819,33071,68094,59424,30609,44708,12753,14657,48898,68673,56,59097,52014,44711,66224,11192,22532,33493,53901,43730,61063,27455,30945,16907,68415,17783,62557,18770,52047,22509,54578,63337,49031,49745,41871,59213,52367,18116,20721,35427,13278,32679,68058,52353,61557,59472,43301,49852,20616,55591,57991,62582,50440,49223,27171,34751,1971,42606,53959,8094,2782,51877,43148,46112,19325,7586,43466,37947,39682,12888,51293,22125,42898,36488,66634,6176,14832,27279,33366,65026,25699,46646,35058,11904,35719,59996,59914,49207,17568,42670,28267,16215,68603,61441,11254,42524,31153,7082,27347,20987,31586,21138,23785,32547,48942,40502,63960,3603,15665,49070,43882,45398,13832,412,2651,35563,63825,64741,6379,38608,6064,36674,230,27276,24584,61332,31037,23094,66574,38906,37842,33438,42022,7374,31360,67060,21220,45616,12052,47601,5143,12043,36458,5125,68953,54820,32646,51933,18090,29707,65844,20818,32624,62197,24744,2964,5421,58672,26351,61274,40053,59984,44528,38128,45899,22833,21492,55581,47689,15171,59280,64163,43550,6441,25474,45817,51693,8378,48623,767,47914,16043,22357,3985,67216,32141,47665,33535,62984,66986,7303,15350,37412,21105,38724,16402,7615,22761,57905,57903,43770,40562,41120,50468,68282,16533,47635,30470,15093,45588,26,20712,20413,20479,3514,65928,48027,705,61329,28746,29040,3158,44862,3108,49732,17057,13464,7413,27660,37891,18495,58971,19662,55075,17963,67015,34531,8325,16958,45349,44774,66220,26870,56686,9361,37456,49946,2030,21426,19125,14800,52689,36308,61928,35559,48969,12163,6553,11507,32704,31334,29653,51043,57305,28617,46063,20923,15786,1792,57063,18952,57364,2978,7141,61179,57846,21574,39706,50815,37745,4118,25471,26657,56079,3675,1304,50225,14374,22350,45856,38957,31582,15335,24764,50663,50694,50941,59751,32417,54418,21350,51253,20795,28894,10978,63342,41234,62239,36449,26082,12173,30319,43471,37121,3982,2554,42633,66156,16140,44655,64501,66297,16889,51265,27086,7475,52538,50761,35610,15123,62445,62632,24715,66018,41702,58320,47983,26373,7236,619,36420,62060,43647,53054,37194,9054,17050,27603,57290,49241,59149,20865,65982,67139,50088,6875,25511,18040,7649,32212,5706,63904,56687,59157,57813,8666,62073,35390,30857,63711,14799,27958,31904
        ,30853,6421,45677,48171,18982,12934,55648,54802,15170,25036,57316,67325,38580,21805,56754,67339,9194,985,31525,43138,9792

    };


    TreeCreate( &usedTree );

   

    for ( int i = 0; i < 420; i++)
    {
        TreeInsert( usedTree, aux[i] );
        printf(" %d ", i);
        //printf(" Iteration step : %d: \n Left Height: %d \n Right Height: %d \n Verdict: \n", i, TreeGetHeightAux( usedTree->Head->Left ), TreeGetHeightAux( usedTree->Head->Right ));
        TestRB(usedTree);
    }

//    TreePrint(usedTree->Head);

    for (int i = 0; i < 420; i++)
    {
        TreeRemove(usedTree, aux[i]);
        printf(" %d ", i);
        TestRB(usedTree);
    }

//    TreeDestroy(&usedTree);

    return 0;
}


int CustomHeapTest()
{
    CC_HEAP* usedHeap = NULL;

    CC_VECTOR* test = NULL;

    // Vector part

    VecCreate(&test);


    VecInsertHead(test, 6);

    VecInsertHead(test, 50);

    VecInsertHead(test, 21);

    VecInsertHead(test, 100);

    VecInsertHead(test, 82);

    VecInsertHead(test, 82);

    VecInsertHead(test, 2);

    // End of vector part

    HpCreateMaxHeap(&usedHeap, test);

    VecPrint(usedHeap->Elements);

    HpSortToVector(usedHeap, test);

    VecPrint(test);

    return 0;
}


int CustomHashTableTest()
{
    CC_HASH_TABLE* hash = NULL;

    HtCreate(&hash);

    for (int i = 0; i < 10; i++)
    {
        HtSetKeyValue(hash, "Hello", i);
        HtSetKeyValue(hash, "Hello2", i);
        HtSetKeyValue(hash, "Hello1", i);
        HtSetKeyValue(hash, "Hello4", i);
    }

    HtPrint(hash);

    return 0;
}