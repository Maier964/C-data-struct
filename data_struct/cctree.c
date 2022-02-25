#include "cctree.h"
#include "common.h"
#include "ccstack.h"


#define GO_LEFT(x) x = x->Left
#define GO_RIGHT(x) x = x->Right
#define BLACK 0
#define RED 1

#define isBlack(x) ( (x) == NULL || (x)->Colour == BLACK )

// Delete
static int notOkRed = 0;


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

int TreeDestroy(CC_TREE **Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    TreeClear(*Tree);

    //free(Tree);

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

        // Added 
        if ( nodeParent == NULL )
        {
            Tree->Head = node;
        }
        //

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

    // Minor fix-up for some particular cases of rotations
    if ( Tree->Head->Parent != NULL )
    {
        Tree->Head->Colour = RED;
        Tree->Head = Tree->Head->Parent;
    }


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
    //bool originalColour;
    PNODE nodeParentAux = NULL;

    if ( TreeGetNode( Tree, Value, &nodeToBeDel ) != 1 )
    {
        LOG_ERROR("Tree doesn't contain value %d", Value);
        return -1;
    }

    // nodeToBeDel is assigned
    start:

    if( nodeToBeDel->Left == NULL || nodeToBeDel->Right == NULL )
    {
        nodeDeleteIterator = nodeToBeDel;
    }    
    else
    {
        TreePredecesor( nodeToBeDel->Right, &nodeDeleteIterator );
    }

    if ( nodeDeleteIterator->Left != NULL )
    {
        nodeAux = nodeDeleteIterator->Left;
    }
    else
    {
        nodeAux = nodeDeleteIterator->Right;
    }

    if ( nodeAux != NULL )
    {
        nodeAux->Parent = nodeDeleteIterator->Parent;
    }

    nodeParentAux = nodeDeleteIterator->Parent;

    bool nodeIteratorIsLeft = false;

    if ( nodeDeleteIterator->Parent == NULL )
    {
        Tree->Head = nodeAux;
    }
    else if ( nodeDeleteIterator == nodeDeleteIterator->Parent->Left )
    {
        nodeDeleteIterator->Parent->Left = nodeAux;
        nodeIteratorIsLeft = true;
    }
    else
    {
        nodeDeleteIterator->Parent->Right = nodeAux;
        nodeIteratorIsLeft = false;
    }

    if ( nodeDeleteIterator != nodeToBeDel )
    {
        nodeToBeDel->Value = nodeDeleteIterator->Value;
    }

    if ( nodeDeleteIterator->Colour == BLACK )
    {
        TreeAdjustColourRemove( Tree, nodeAux, nodeParentAux, nodeIteratorIsLeft );
    }

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

    if ( NULL == Tree )
    {
        return -1;
    }

    if ( Index > Tree->Count )
    {
        return -1;
    }

    return 0;
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

    NODE* y = Source->Right;

    Source->Right = y->Left;
    if ( y->Left != NULL )
    {
        y->Left->Parent = Source;
    }
    y->Parent = Source->Parent;

    // 3 cases
    // 1. Source is root
    if ( Source->Parent == NULL )
    {
        Tree->Head = y;
    }
    // 2. Source is the left child of its parent
    else if ( Source == Source->Parent->Left ){
        Source->Parent->Left = y;
    }
    // 3. Source is the right child of its parent
    else Source->Parent->Right = y;

    y->Left = Source;

    Source->Parent = y;

    return 0;
}

int TreeRightRotate( PCC_TREE Tree, PNODE Source )
{

    if ( NULL == Tree )
    {
        return -1;
    }

    NODE* y = Source->Left;


    Source->Left = y->Right;
    if ( y->Right != NULL )
    {
        y->Right->Parent = Source;
    }

    y->Parent = Source->Parent;

    if ( Source->Parent == NULL )
    {
        Tree->Head = y;
    }
    else if ( Source == Source->Parent->Right )
    {
        Source->Parent->Right = y;
    }
    else Source->Parent->Left = y;

    y->Right = Source;

    Source->Parent = y;

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


int TreeAdjustColourRemove(PCC_TREE Tree, PNODE Node, PNODE NodeParent, bool isLeft)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Node);
    CC_UNREFERENCED_PARAMETER(NodeParent);
    CC_UNREFERENCED_PARAMETER(isLeft);

    if (NULL == Tree)
    {
        return -1;
    }

    // Continue from here tomorrow. Hopefully you remember
    PNODE sibling = NULL;


    while( Node != Tree->Head && isBlack(Node)  )
    {
        if ( isLeft )
        {
            sibling = NodeParent->Right;
            if ( sibling->Colour == RED )
            {
                sibling->Colour = BLACK;
                NodeParent->Colour = RED;
                TreeLeftRotate( Tree, NodeParent );
                sibling = NodeParent->Right;
            }

            if ( isBlack(sibling->Left)  && isBlack(sibling->Right) )
            {
                sibling->Colour = RED;
                Node = NodeParent;
                NodeParent = Node->Parent;
                isLeft = ( NodeParent != NULL &&  Node == NodeParent->Left );
            }
            else
            {
                if ( isBlack( sibling->Right ) )
                {
                    sibling->Left->Colour = BLACK;
                    sibling->Colour = RED;
                    TreeRightRotate( Tree, sibling );
                    sibling = NodeParent->Right;
                }

                sibling->Colour = NodeParent->Colour;
                NodeParent->Colour = BLACK;

                if ( sibling->Right != NULL )
                {
                    sibling->Right->Colour = BLACK;
                }
                TreeLeftRotate( Tree, NodeParent );
                Node = Tree->Head;
                NodeParent = NULL;
            }
        }
        else
        // isLeft = False; symmetric
        {
            sibling = NodeParent->Left;
            if ( sibling->Colour == RED )
            {
                sibling->Colour = BLACK;
                NodeParent->Colour = RED;
                TreeRightRotate( Tree, NodeParent );
                sibling = NodeParent->Left;
            }

            if ( isBlack(sibling->Right) && isBlack(sibling->Left))
            {
                sibling->Colour = RED;
                Node = NodeParent;
                NodeParent = Node->Parent;
                isLeft = (NodeParent != NULL && Node == NodeParent->Left);
            }
            else
            {
                if ( isBlack(sibling->Left) )
                {
                    sibling->Right->Colour = BLACK;
                    sibling->Colour = RED;
                    TreeLeftRotate( Tree, sibling );
                    sibling = NodeParent->Left; 
                }

                sibling->Colour = NodeParent->Colour;
                NodeParent->Colour = BLACK;
                if ( sibling->Left != NULL )
                {
                    sibling->Left->Colour = BLACK;
                }
                TreeRightRotate( Tree, NodeParent );
                Node = Tree->Head;
                NodeParent = NULL;
            }
        }

        
    }

    if (Node != NULL)
    {
        Node->Colour = BLACK;
    }
    

    return 0;
}


int TestRB( PCC_TREE Root )
{

    if (Root->Head == NULL)
        return 0;

    if ( Root->Head->Colour != BLACK )
    {
        printf("\nNot RB!! Condition 1 is wrong omggg....");
        return 0;
    }

    if ( GetRBHeight( Root->Head ) == 0 )
    {
        printf("%d ", GetRBHeight(Root->Head->Right) - GetRBHeight(Root->Head->Right));
        printf("\nNot RB!! Condition 2 is wrong omggg....");
        return 0;
    }

    CheckRedCondition( Root->Head );

    if ( notOkRed != 0 )
    {
        printf("\nNot RB!! Condition 3 is wrong omggg....");
        return 0;
    }


    printf( "YESSS, RB TREE!!!\n" );

    
    return 0;

}


int GetRBHeight( PNODE Node )
{
    if ( Node == NULL )
    {
        return 1;
    }

    int leftBHeight = GetRBHeight(Node->Left);

    if ( leftBHeight == 0 )
        return leftBHeight;

    int rightBHeight = GetRBHeight( Node->Right );


    if ( rightBHeight == 0 )
        return rightBHeight;

    if ( leftBHeight != rightBHeight )
        return 0;
    else return  leftBHeight + 1 - Node->Colour;
    
}

int CheckRedCondition( PNODE Node )
{

    if ( Node == NULL )
    {
        return 0;
    }

    if ( Node->Colour == RED )
    {
        if ( (Node->Left!= NULL && Node->Left->Colour != BLACK)
         || (Node->Right!= NULL && Node->Right->Colour != BLACK) )
        {
            notOkRed++;
        }
    }
    else
    {
        CheckRedCondition( Node -> Left );
        CheckRedCondition( Node ->Right);
    }

    return 0;
}