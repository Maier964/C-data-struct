#include "cctree.h"
#include "common.h"


#define GO_LEFT(x) x = x->Left
#define GO_RIGHT(x) x = x->Right
#define BLACK 0
#define RED 1


int TreeCreate(CC_TREE **Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if ( NULL == Tree )
    {
        return -1;
    }

    PCC_TREE treeTemplate = (PCC_TREE) malloc( sizeof( CC_TREE ) );

    treeTemplate->Head = NULL;
    treeTemplate->Count = 0;

    *Tree = treeTemplate;

    return 0;
}

// Leave for later
int TreeDestroy(CC_TREE **Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    TreeClear(*Tree);

    free(Tree);

    Tree = NULL;

    return 0;
}

int TreeInsert(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if ( NULL == Tree )
    {
        return -1;
    }

    PNODE node = NULL;
    PNODE nodeIterator = NULL;
    PNODE nodeParent = NULL;

    node = (PNODE)malloc(sizeof(NODE));

    if ( TreeNodeCreate( node, Value ) )
    {
        LOG_ERROR("Node creation failed (Value : %d).", Value);
        return -1;
    }

    // Tree is empty
    if ( Tree->Head == NULL )
    {
        Tree->Head = node;
    }
    else
    {
        nodeIterator = Tree->Head;

        while( nodeIterator != NULL )
        {
            nodeParent = nodeIterator;

            if ( nodeIterator->Value <= Value )
                GO_RIGHT(nodeIterator);
            else GO_LEFT(nodeIterator);
        }

        // Found address of new node. We also got his parent with the help of nodeParent

        node->Parent = nodeParent;

        if ( nodeParent->Value > Value )
        {
            nodeParent->Left = node;
        }
        else
        {
            nodeParent->Right = node;
        }
    }

    TreeAdjustColourInsert(Tree, node);

    Tree->Count++;

    return 0;
}

int TreeRemove(CC_TREE *Tree, int Value)
{
    // Avand doar value ca reprezentant si fiind posibile duplicate, ii cam ciudat de facut removal-ul pentru ca 
    // removal-ul va sterge cea mai apropiata valoare din cele duplicate. 

    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    PNODE nodeToBeDel = NULL;
    PNODE nodeDeleteIterator = NULL;
    PNODE nodeAux = NULL;

    if ( TreeGetNode( Tree, Value, &nodeToBeDel ) != 1 )
    {
        LOG_ERROR("Tree doesn't contain value %d", Value);
        return -1;
    }

    // nodeToBeDel is assigned
    start:

    nodeDeleteIterator = nodeToBeDel;

    bool originalColour = nodeDeleteIterator->Colour;

    // Is a leaf or has some right childs. In both cases, TreeReplaceSubtree does the job of interchanging the two nodes
    // We are sure of the fact that if a node has no left child, it will have only one right child, because otherwise the tree is not balanced anymore.
    if ( nodeToBeDel->Left == NULL ) 
    {
        nodeAux = nodeToBeDel->Right;
        TreeReplaceSubtree( Tree, nodeToBeDel, nodeToBeDel->Right );
    }
    else
    {
        if ( nodeToBeDel->Right == NULL ) // Has only a left child
        {
            nodeAux = nodeToBeDel->Left;
            TreeReplaceSubtree( Tree, nodeToBeDel, nodeToBeDel->Left );
        }
        else{
            // Has both children
            TreePredecesor( nodeToBeDel->Right, &nodeDeleteIterator);
            originalColour = nodeDeleteIterator->Colour;

            nodeAux = nodeDeleteIterator->Right;


            if ( nodeDeleteIterator->Parent != nodeToBeDel ) 
            {
                TreeReplaceSubtree( Tree, nodeDeleteIterator, nodeDeleteIterator->Right );
                nodeDeleteIterator->Right = nodeToBeDel->Right;
                nodeDeleteIterator->Right->Parent = nodeDeleteIterator;
            }

            TreeReplaceSubtree( Tree, nodeToBeDel, nodeDeleteIterator );

            nodeDeleteIterator->Left = nodeToBeDel->Left;

            nodeDeleteIterator->Left->Parent = nodeDeleteIterator;

            nodeDeleteIterator->Colour = nodeToBeDel->Colour;
        }    
    }

    if ( originalColour == BLACK )
        TreeAdjustColourRemove( Tree, nodeAux );

    Tree->Count--;

    // ALL values must be deleted -> Check for duplicates
    if ( TreeGetNode( Tree, Value, &nodeToBeDel ) == 1 )
    {
        goto start;
    }

    return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

     PNODE nodeIterator = NULL;

    if ( NULL == Tree )
    {
        return -1;
    }

    nodeIterator = Tree->Head;


    while( nodeIterator != NULL )
    {
        if ( nodeIterator->Value == Value )
        {
            // Value found
            return 1;
        }
        
        if ( nodeIterator->Value >= Value )
        {
            GO_RIGHT(nodeIterator);
        }
        else GO_LEFT(nodeIterator);
    }

    // Tree doesn't contain value
    return 0;
}

int TreeGetCount(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if ( NULL == Tree )
    {
        return -1;
    }

    return Tree->Count;
}

int TreeGetHeight(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if ( NULL == Tree )
    {
        return -1;
    }

    // Compute Tree->Height; 

    return TreeGetHeightAux( Tree->Head );
}

int TreeGetHeightAux( PNODE Node )
{
    CC_UNREFERENCED_PARAMETER(Node);

    if ( Node == NULL )
    {
        return 0;
    }

    return 1 + Max( TreeGetHeightAux( Node->Left ), TreeGetHeightAux( Node->Right ) );
}

int TreeClear(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    TreeClearAux(Tree->Head);

    Tree->Head = NULL;
    Tree->Count = 0;

    return 0;

}

void TreeClearAux( PNODE Head )
{
    if (Head == NULL)
    {
        return;
    }

    TreeClearAux( Head->Left );
    TreeClearAux( Head->Right );

    free(Head);
    
}

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    return -1;
}

int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    return -1;
}

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);
    return -1;
}


int TreeLeftRotate( PCC_TREE Tree, PNODE Source )
{

    if ( NULL == Tree )
    {
        return -1;
    }

    NODE* Right = Source->Right;

    Source->Right = Right->Left;
    if ( Right->Left != NULL )
    {
        Right->Left->Parent = Source;
    }
    Right->Parent = Source->Parent;

    // 3 cases
    // 1. Source is root
    if ( Source->Parent == NULL )
    {
        Tree->Head = Right;
    }
    // 2. Source is the left child of its parent
    else if ( Source == Source->Parent->Left ){
        Source->Parent->Left = Right;
    }
    // 3. Source is the right child of its parent
    else Source->Parent->Right = Right;

    Right->Left = Source;

    Source->Parent = Right;

    return 0;
}

int TreeRightRotate( PCC_TREE Tree, PNODE Source )
{

    if ( NULL == Tree )
    {
        return -1;
    }

    NODE* Left = Source->Left;


    Source->Left = Left->Right;
    if ( Left->Right != NULL )
    {
        Left->Right->Parent = Source;
    }

    Left->Parent = Source->Parent;

    if ( Source->Parent == NULL )
    {
        Tree->Head = Source;
    }
    else if ( Source->Parent == Source->Parent->Left )
    {
        Source->Parent->Left = Left;
    }
    else Source->Parent->Right = Left;

    Left->Right = Source;

    Source->Parent = Left;

    return 0;
}


int TreeNodeCreate( PNODE Node, int Value )
{
    Node->Value = Value;
    Node->Colour = RED; // Inserted nodes are red by default.
    Node->Left = Node->Right = Node->Parent = NULL;

    return 0;
}



int TreeAdjustColourInsert( PCC_TREE Tree, PNODE Node )
{
    // This function is only accesed in RBInsert where we already perform this check, 
    // but this function can be accesed independently if some memory overflow is present in the program,
    // so double-checking is advised. (is this relevant????? "Double-check" before commiting to this comment)
    if ( NULL == Tree )
    {
        return -1;
    }

    PNODE nodeIterator = NULL;

    // A red node cannot have a red parent. 
    while ( Node->Parent != NULL && Node->Parent->Colour == RED ) // patched this with first condition
    {
        // It is mandatory that if the parent of a node is red,
        // that node must have a depth of at least 2. 
        // Hence we can dereferenciate a second parent pointer with no worries.
        if ( Node->Parent == Node->Parent->Parent->Left )
        {
            nodeIterator = Node->Parent->Parent->Right;
            if ( nodeIterator != NULL &&  nodeIterator->Colour == RED ) // patched this with first condition
            {
                Node->Parent->Colour = BLACK;
                nodeIterator->Colour = BLACK;
                Node->Parent->Parent->Colour = RED;
                Node = Node->Parent->Parent;
            }
            else
            {
                if ( Node == Node->Parent->Right )
                {
                    Node = Node->Parent;
                    TreeLeftRotate( Tree, Node );
                }

                Node->Parent->Colour = BLACK;

                Node->Parent->Parent->Colour = RED;

                TreeRightRotate( Tree, Node->Parent->Parent );
            }
        }
        else 
        { // Same idea but inversed direction
            nodeIterator = Node->Parent->Parent->Left;
            if (nodeIterator != NULL && nodeIterator->Colour == RED ) // patched this aswell
            {
                Node->Parent->Colour = BLACK;
                nodeIterator->Colour = BLACK;
                Node->Parent->Parent->Colour = RED;
                Node = Node->Parent->Parent;
            }
            else
            {
                if ( Node == Node->Parent->Left )
                {
                    Node = Node->Parent;
                    TreeRightRotate( Tree, Node );
                }

                Node->Parent->Colour = BLACK;

                Node->Parent->Parent->Colour = RED;

                TreeLeftRotate( Tree, Node->Parent->Parent );
            }
        }
    }

    // Root of a RB tree MUST be black.
    Tree->Head->Colour = BLACK;

    return 0;
}


int TreePrint( PNODE Head )
{
    // DO a simple preorder for now..

    if (Head == NULL)
    {
        return 0;
    }

    printf("%d %d,", Head->Value, Head->Colour);

    TreePrint ( Head->Left );

    TreePrint ( Head->Right );
    

    return 0;
}

int TreeGetNode( PCC_TREE Tree, int Value, PNODE* node )
{
    PNODE nodeIterator = NULL;

    if ( NULL == Tree )
    {
        return -1;;
    }

    nodeIterator = Tree->Head;

    while( nodeIterator != NULL )
    {
        if ( nodeIterator->Value == Value)
        {
            // Found
            *node = nodeIterator;
            return 1;
        }

        if ( nodeIterator->Value > Value )
        {
            GO_LEFT( nodeIterator );
        }
        else
        {
            GO_RIGHT( nodeIterator );
        }
    }
    // Not found
    return 0;
}


int TreeReplaceSubtree( PCC_TREE Tree, PNODE Source, PNODE Subtree )
{

    if ( Source->Parent == NULL )
    {
        Tree->Head = Subtree;
    }
    else
    {
        if ( Source == Source->Parent->Left )
        {
            Source->Parent->Left = Subtree;
        }
        else
        {
            Source->Parent->Right = Subtree;
        } 
    } 
    // AICI E BUBA!!!
    if (Subtree != NULL)
    {
        Subtree->Parent = Source->Parent;
    }
     

    return 0;
}


int TreePredecesor( PNODE Node, PNODE* Predecesor ) // This is actually successor of x.. We try to find the predecesor of x->right which is the successor of x.
{
    if ( NULL == Node )
    {
        return -1;
    }

    PNODE nodeIterator = Node;

    while( nodeIterator->Left != NULL )
    {
        GO_LEFT( nodeIterator );
    }

    *Predecesor = nodeIterator;

    return 0;
}


int TreeAdjustColourRemove( PCC_TREE Tree, PNODE Node )
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Node);

    if ( NULL == Tree || NULL == Node )
    {
        return -1;
    }

    PNODE sibling = NULL;

    while( Node != Tree->Head && Node->Colour == BLACK )
    {
        if ( Node == Node->Parent->Left )
        {
            sibling = Node->Parent->Right;

            // Case 1
            if ( sibling->Colour == RED )
            {
                sibling->Colour = BLACK;
                Node->Parent->Colour = RED;
                TreeLeftRotate( Tree, Node->Parent );
                sibling = Node->Parent->Right;
            }
            // Case 2
            if ( sibling->Left->Colour == BLACK && sibling->Right->Colour == BLACK )
            {
                sibling->Colour = RED;
                Node = Node->Parent;
            }
            else
            {
                // Case 3
                if ( sibling->Right->Colour == BLACK )
                {
                    sibling->Left->Colour = BLACK;
                    sibling->Colour = RED;
                    TreeRightRotate( Tree, sibling );
                    sibling = Node->Parent->Right;
                } 

                // Case 4
                sibling->Colour = Node->Parent->Colour;

                Node->Parent->Colour = BLACK;

                sibling->Right->Colour = BLACK;

                TreeLeftRotate( Tree, Node->Parent );

                Node = Tree->Head;
            }
        }
        else // Symmetric
        {
            sibling = Node->Parent->Left;
            if ( sibling->Colour == RED )
            {
                sibling->Colour = BLACK;
                Node->Parent->Colour = RED;
                TreeRightRotate( Tree, Node->Parent );
                sibling = Node->Parent->Left;
            }
            if ( sibling->Right->Colour == BLACK && sibling->Left->Colour == BLACK )
            {
                sibling->Colour = RED;
                Node = Node->Parent;
            }
            else
            {
                if ( sibling->Left->Colour == BLACK )
                {
                    sibling->Right->Colour = BLACK;
                    sibling->Colour = RED;
                    TreeLeftRotate( Tree, sibling );
                    sibling = Node->Parent->Left;
                } 

                sibling->Colour = Node->Parent->Colour;

                Node->Parent->Colour = BLACK;

                sibling->Left->Colour = BLACK;

                TreeRightRotate( Tree, Node->Parent );

                Node = Tree->Head;
            }
        }
    }

    Node->Colour = BLACK;

    return 0;
}