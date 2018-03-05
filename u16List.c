// Sorted list with specified max length.
// Stores the largest MAXLEN number of values.

#include "stdlib.h"
#include "u16List.h"

//#define U16LIST_DBG

u16Node* getNewNode(uint16 val);
boolean deleteNode(u16Node **prev_pp, u16Node **curr_pp);
void initHead(uint16 val);
void deleteList();
void fixListLen();

void printList();


static u16Node *head = NULL;
static boolean isInitialized = FALSE; 
static uint32 MAXLEN = 0;
static uint32 LISTLEN = 0;

/****************************************************************/
/*!
@brief
Initialized list's max length. 
User can only initialize once.

@param maxLen
Length of the list

@return boolean
True:if value is set, False:else
*/
boolean initList(uint32 maxLen)
{
    // if not set yet.
    if (isInitialized == FALSE)
    {
        MAXLEN = maxLen;
        LISTLEN = 0;
        isInitialized = TRUE;
    }
    #ifdef U16LIST_DBG
    printf("initList - maxLen %d, MAXLEN %d\n", maxLen, MAXLEN);
    #endif
    return isInitialized;
}

/****************************************************************/
/*!
@brief
Sorts input value. Discards if too small.

@param val
Value to be sorted

*/
void sortValue(uint16 val)
{
    u16Node *curr_p = NULL;
    u16Node *next_p = NULL;
    u16Node *node = NULL;
    #ifdef U16LIST_DBG
    printf("\n----sortValue - val %d\n", val);
    #endif
    //printList();
    if(head == NULL)
    {
        initHead(val);
        printList();
    }
    else
    {
        next_p = head;
        printList();
        while(next_p->val < val)
        {
            curr_p = next_p;
            next_p = next_p->next;
            if(next_p == NULL) break;
        }
        
        node = getNewNode(val);
       
        if(node != NULL)
        {
            #ifdef U16LIST_DBG
            printf("curr_p 0x%x, next_p 0x%x\n", (uint32)curr_p, (uint32)next_p);
            #endif
            if(curr_p == NULL) // insert at head
            {
                node->next = head;
                head = node;
            }
            else
            {
                curr_p->next = node;
            }
            node->next = next_p;
            fixListLen();
        }
    }
}

/****************************************************************/
/*!
@brief
Resets the list and performs cleanup.

*/
void resetList()
{
    deleteList();
    isInitialized = FALSE;
}

/****************************************************************/
/*!
@brief
Allocates mem and gets new node

@param 
value to set for new node.

@return New u16Node

*/
u16Node* getNewNode(uint16 val)
{
    u16Node *node = NULL;
    node = (u16Node*)malloc(sizeof(u16Node));
    #ifdef U16LIST_DBG
    printf("getNewNode - val %d\n", val);
    #endif
    if(node == NULL)
    {
        printf("getNewNode - Out of memory.\n");
    }
    else
    {
        node->val = val;
        node->next = NULL;
        LISTLEN++;
    }
    #ifdef U16LIST_DBG
    printf("getNewNode - LISTLEN %d\n", LISTLEN);
    #endif
    return node;
}

/****************************************************************/
/*!
@brief
Deletes current node.

@param prev_p
Pointer to previous node.

@param curr_p
Pointer to current node.

@return 
TRUE: if deleted current node, FALSE: else
*/
boolean deleteNode(u16Node **prev_pp, u16Node **curr_pp)
{
    u16Node *delNode = *curr_pp;
    
    if((curr_pp == NULL) || (*curr_pp == NULL))
        return FALSE;
    
    // if prev_p == NULL, curr_p is head
    if((prev_pp == NULL) || (*prev_pp == NULL))
    {
        head = (*curr_pp)->next;
        #ifdef U16LIST_DBG
        if(head != NULL)
        {
            printf("deleteNode - Reset head val %d\n", head->val);
        }
        #endif
    }
    else
    {
        (*prev_pp)->next = (*curr_pp)->next;
        #ifdef U16LIST_DBG
        printf("deleteNode - split \n");
        #endif
    }
    // delete node
    LISTLEN--;
    #ifdef U16LIST_DBG
    if(head != NULL)
    {
        printf("deleteNode - Deleting value %d, head value %d\n", delNode->val, head->val);
    }

    #endif
    free(delNode);
    #ifdef U16LIST_DBG
    printf("deleteNode - LISTLEN %d\n", LISTLEN);
    #endif
    return TRUE;
}

/****************************************************************/
/*!
@brief
Initialize head node.
Deletes any additional nodes.

@param val
Value to initialize head with.

*/
void initHead(uint16 val)
{
    #ifdef U16LIST_DBG
    printf("initHead\n");
    #endif
    deleteList();
    head = getNewNode(val);
}


/****************************************************************/
/*!
@brief
Deletes the list.

*/
void deleteList()
{
    #ifdef U16LIST_DBG
    printf("deleteList\n");
    #endif
    while(head != NULL)
    {
        deleteNode(NULL, &head);
    }
}

/****************************************************************/
/*!
@brief
Checks list length.
if LISTLEN > MAXLEN, deletes old head and assigns new head.

*/
void fixListLen()
{
    #ifdef U16LIST_DBG
    printf("fixListLen LISTLEN %d, MAXLEN %d\n", LISTLEN, MAXLEN);
    #endif
    if(LISTLEN > MAXLEN)
    {
        #ifdef U16LIST_DBG
        printf("fixListLen - call delete for val %d\n", head->val);
        #endif
        deleteNode(NULL, &head);
        fixListLen();
    }
}

/****************************************************************/
/*!
@brief
Prints list to console
*/
void printList()
{
    #ifdef U16LIST_DBG
    printf("List - ");
    u16Node *node = head;
    while(node != NULL)
    {
        printf("%d ", node->val);
        node = node->next;
    }
    printf("\n");
    #endif
}

/****************************************************************/
/*!
@brief
Prints list to file.

@param fh
Pointer to file handle.
*/
void printListToFile(FILE **fh)
{
    u16Node *node = head;
    while(node != NULL)
    {
        fprintf(*fh, "%d\n", node->val);
        node = node->next;
    }
}


