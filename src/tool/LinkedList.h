#ifndef SHIBETA_CHESS_LINKED_LIST
#define SHIBETA_CHESS_LINKED_LIST

typedef struct LinkedList LinkedList;
typedef struct LinkedListNode LinkedListNode;

struct LinkedList{
    unsigned short length;

    LinkedListNode* head;
    LinkedListNode* tail;

};

struct LinkedListNode{
    void* node;

    LinkedListNode* next;
    LinkedListNode* last;

};

LinkedList* LinkedList_NewInstance();
LinkedList* LinkedList_NewInstanceWithNodes(void*[], int);

LinkedListNode* LinkedList_GetHead(LinkedList*);
LinkedListNode* LinkedList_GetTail(LinkedList*);
LinkedListNode* LinkedList_GetNode(LinkedList*, int);

unsigned short LinkedList_GetLength(LinkedList*);

void LinkedList_AddToHead(LinkedList*, void*);
void LinkedList_AddToTail(LinkedList*, void*);
void LinkedList_AddNode(LinkedList*, void*, int);

void LinkedList_RemoveHead(LinkedList*);
void LinkedList_RemoveTail(LinkedList*);
void LinkedList_RemoveNode(LinkedList*,int);

void LinkedList_Free(LinkedList*);

LinkedListNode* LinkedListNode_NewInstance(void*);

void LinkedListNode_SetNextNode(LinkedListNode*,void*);
void LinkedListNode_SetLastNode(LinkedListNode*,void*);

LinkedListNode* LinkedListNode_GetNextNode(LinkedListNode*);
LinkedListNode* LinkedListNode_GetLastNode(LinkedListNode*);

void LinkedListNode_Free(LinkedListNode*);

#endif
