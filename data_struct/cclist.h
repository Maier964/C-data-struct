#pragma once

typedef struct _LIST_NODE
{
    int Value;
    char* Key;
    struct _LIST_NODE* Next;
}LIST_NODE, *PLIST_NODE;

typedef struct _CC_LIST
{
    LIST_NODE* Head;
}CC_LIST, *PCC_LIST;


int ListCreate( PCC_LIST* );

int ListDelete( PCC_LIST* );

int ListAdd( PCC_LIST, PLIST_NODE );

int CreateListNode( PLIST_NODE, int , char* );

