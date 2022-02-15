#pragma once

// Ma mai gandesc daca il folosesc..
typedef void(*PFUNC_Cleanup)(void**);

typedef void(*PFUNC_ForEach)(void*);

typedef struct _NODE
{
    struct _NODE* Flink;
    struct _NODE* Blink;
    void* Data;
} NODE, * PNODE;

typedef struct _LIST
{
    NODE Head;
    PFUNC_Cleanup CleanupFunc;
    PFUNC_ForEach Func;

} LIST, * PLIST;

void LstInit(PLIST* Lst, PFUNC_Cleanup CleanupFunc);
void LstUninit(PLIST* Lst);

int LstIsEmpty(PLIST Lst);
int LstPopFront(PLIST lst);
int LstInsertFront(PLIST lst, void* data);

int LstForEach(PLIST Lst, PFUNC_ForEach Func);

