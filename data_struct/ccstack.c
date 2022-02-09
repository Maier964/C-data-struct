#include "ccstack.h"
#include "ccvector.h"
#include "common.h"

#define INITIAL_SIZE 100

int StCreate(CC_STACK **Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    CC_STACK* stackTemplate = NULL;

    if (Stack == NULL)
        return -1;

    stackTemplate = (CC_STACK*)malloc(sizeof(CC_STACK));

    VecCreate( &stackTemplate->Vector );


    *Stack = stackTemplate;


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

    VecDestroy( &stack->Vector );

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

    if ( NULL == Stack || StIsEmpty(Stack) )
    {
        return -1;
    }

    if ( (StPeek(Stack, Value)) )
    {
        // MACRO LOGGING WITH SPECIFIC MESSAGE
        return -1;
    }

    // StPeek worked ok (Ret_Value == 0)
    VecRemoveByIndex( Stack->Vector, 0 );

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

    // Empty stack
    if ( StIsEmpty(Stack) )
    {
        return -1;
    }

    *Value = Stack->Vector->Array[0];

    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if ( NULL == Stack )
    {
        return -1;
    }

    return ( Stack->Vector->Count ) ? 0 : 1;
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


int StPushStack( CC_STACK* Stack, CC_STACK* StackToPush )
{

    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(StackToPush);

    if ( NULL == Stack || NULL == StackToPush )
    {
        return -1;
    }


    for( int i = 0; i < StackToPush->Vector->Count; i++ )
    {
        VecInsertTail( Stack->Vector , StackToPush->Vector->Array[i] );
    }

    VecClear( StackToPush->Vector );


    return 0;
}

void StPrint( CC_STACK* Stack )
{
    CC_UNREFERENCED_PARAMETER(Stack);
    
    // Stack not empty
    if (!StIsEmpty(Stack))
    {
        VecPrint( Stack->Vector );
    }
    else printf("Empty stack.\n");
}
