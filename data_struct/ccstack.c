#include "ccstack.h"
#include "ccvector.h"
#include "common.h"

#define INITIAL_SIZE 100

int StCreate(CC_STACK **Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    CC_STACK* Stack_template = NULL;

    if (Stack == NULL)
        return -1;

    Stack_template = (CC_STACK*)malloc(sizeof(CC_STACK));


    *Stack = Stack_template;

    return 0;
}

int StDestroy(CC_STACK **Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    PCC_STACK stack = *Stack;

    if ( NULL == stack )
    {
        return -1;
    }

    VecDestroy( stack->Vector );

    free( stack );

    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Stack )
    {
        return -1;    
    }

    VecInsertHead( Stack->Vector , Value );

    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Stack )
    {
        return -1;
    }


    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Stack )
    {
        return -1;
    }

    return Stack->Vector->Array[0];
}

int StIsEmpty(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    // Too fancy maybe? idk
    return ( Stack->Vector->Array[0] == NULL) ? 1 : 0;
}

int StGetCount(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    return Stack->Vector->Count;
}

int StClear(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    VecClear( Stack->Vector );

    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(StackToPush);

    for ( int i = 0; i < StackToPush->Vector->Count ; i++ ) // Change this for to look better and be more clear
    {
        VecInsertAfterIndex( Stack->Vector, Stack->Vector->Count, StackToPush->Vector->Array[0] );
        VecRemoveByIndex( StackToPush->Vector, 0 );
    }



    return 0;
}
