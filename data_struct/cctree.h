#pragma once
#include <stdbool.h>
#include "ccvector.h"

typedef struct _NODE{
    int Value;
    struct _NODE* Left;
    struct _NODE* Right;
    struct _NODE* Parent;
    bool Colour; // see if you can optimise this..
    // 0 - Black, 1 - Red
} NODE, *PNODE;

typedef struct _CC_TREE {
    NODE* Head; 
    int Count;
} CC_TREE, *PCC_TREE;

int TreeCreate(CC_TREE **Tree);
int TreeDestroy(CC_TREE **Tree);

// Duplicates are allowed
int TreeInsert(CC_TREE *Tree, int Value);

// Removes all elements equal to Value
int TreeRemove(CC_TREE *Tree, int Value);


//  Returns:
//       1  - Tree contains Value
//       0  - Tree does not contain Value
//      -1  - Error or invalid parameter
int TreeContains(CC_TREE *Tree, int Value);

// Returns the number of elements in Tree or -1 in case of error or invalid parameter
int TreeGetCount(CC_TREE *Tree);

// Returns the height of Tree or -1 in case of error or invalid parameter
int TreeGetHeight(CC_TREE *Tree);

// Removes every element of the tree
int TreeClear(CC_TREE *Tree);

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value);
int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value);
int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value);

// Rotation used for balancing the tree
int TreeLeftRotate( PCC_TREE Tree, PNODE SupportNode );

// Rotation used for balancing the tree
int TreeRightRotate(PCC_TREE Tree, PNODE SupportNode  );

// Auxiliary function that allocates memory for a node
// Returns -1 if invalid parameters
int TreeNodeCreate( PNODE , int );

// Function to recolour/rebalance the tree after insertion
int TreeAdjustColourInsert( PCC_TREE, PNODE );

int TreePrint( PNODE Head );

// Searches for a node and returns it as a parameter
int TreeGetNode( PCC_TREE, int, PNODE* );

int TreePredecesor( PNODE, PNODE* );

int TreeAdjustColourRemove( PCC_TREE, PNODE , PNODE, bool);

int TreeGetHeightAux( PNODE );

// Used to recursively deallocate all nodes
void TreeClearAux(PNODE);

int TestRB( PCC_TREE );

int GetRBHeight( PNODE );

int CheckRedCondition( PNODE );

// Used to create vector of preoder traversal..
int PreoderAux( PNODE, PCC_VECTOR );

int PostorderAux( PNODE, PCC_VECTOR );

int InorderAux( PNODE, PCC_VECTOR );