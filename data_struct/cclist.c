#include "common.h"
#include "cclist.h"


int ListCreate( PCC_LIST* List )
{
    if ( NULL == List )
    {
        return -1;
    }

    PCC_LIST listTemp = (PCC_LIST)malloc( sizeof(CC_LIST) );

    listTemp->Head = NULL;

    *List = listTemp;

    return 0;
}

int ListDelete( PCC_LIST* List )
{
    if ( NULL == List )
    {
        return -1;
    }

    free( *List );

    (*List)->Head = NULL;

    free( List );

    return 0;
}

int CreateListNode( PLIST_NODE Node, int Value, char* Key )
{  
    // Node should be initialised here, so null values accepted
    if ( NULL != Node )
    {
        // Node already initialised
        return -1;
    }
    Node = (PLIST_NODE)malloc(sizeof(LIST_NODE));

    Node->Key = (char*)malloc( GetStringLen(Key) * sizeof(char) );

    Node->Value = Value;

    Node->Key = Key;

    return 0;
}

