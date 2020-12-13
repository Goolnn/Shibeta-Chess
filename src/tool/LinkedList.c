#include <stdlib.h>

#include "LinkedList.h"

LinkedList* LinkedList_NewInstance(){
    LinkedList* instance = (LinkedList*)calloc(1, sizeof(LinkedList));

    instance->length = 0;

    instance->head = NULL;
    instance->tail = NULL;

    return instance;

}

LinkedList* LinkedList_NewInstanceWithNodes(void* nodes[], int length){
    LinkedList* instance = LinkedList_NewInstance();

    if(length != 0){
        for(int i = 0; i < length; i++){
            LinkedList_AddToTail(instance, nodes[i]);

        }

        return instance;

    }

    return LinkedList_NewInstance();

}

LinkedListNode* LinkedList_GetHead(LinkedList* linkedList){
    return linkedList->head;

}

LinkedListNode* LinkedList_GetTail(LinkedList* linkedList){
    return linkedList->tail;

}

LinkedListNode* LinkedList_GetNode(LinkedList* linkedList, int index){
    if(LinkedList_GetLength(linkedList) != 0){
        LinkedListNode* node = LinkedList_GetHead(linkedList);

        for(int i=0; i < index; i++){
            node = node->next;

        }

        return node;

    }else{
        return NULL;

    }

}

unsigned short LinkedList_GetLength(LinkedList* linkedList){
    return linkedList->length;

}

void LinkedList_AddToHead(LinkedList* linkedList, void* node){
    LinkedListNode* head = LinkedListNode_NewInstance(node);

    if(LinkedList_GetLength(linkedList) == 0){
        linkedList->head = head;
        linkedList->tail = head;

    }else{
        linkedList->head->last = head;
        head->next = linkedList->head;

        linkedList->head = head;

    }


    linkedList->length++;

}

void LinkedList_AddToTail(LinkedList* linkedList, void* node){
    LinkedListNode* tail = LinkedListNode_NewInstance(node);

    if(LinkedList_GetLength(linkedList) == 0){
        linkedList->head = tail;
        linkedList->tail = tail;

    }else{
        linkedList->tail->next = tail;
        tail->last = linkedList->tail;

        linkedList->tail = tail;

    }

    linkedList->length++;

}

void LinkedList_AddNode(LinkedList* linkedList, void* node, int index){
    if(index <= LinkedList_GetLength(linkedList) - 1){

    }

}

void LinkedList_RemoveHead(LinkedList* linkedList){
    linkedList->head->next->last=(void*)0;

    linkedList->length--;

}

void LinkedList_RemoveTail(LinkedList* linkedList){
    linkedList->tail->last->next=(void*)0;

    linkedList->length--;

}

void LinkedList_RemoveNode(LinkedList* linkedList,int index){
    LinkedListNode* node;

    if(index == 0){
        node=LinkedList_GetHead(linkedList);

        node->next->last=(void*)0;

    }else if(index==LinkedList_GetLength(linkedList)-1){
        node=linkedList->tail;

        node->last->next=(void*)0;

    }else{
        node=LinkedList_GetNode(linkedList,index);

        node->last=node->next;

    }

    linkedList->length--;

}


LinkedListNode* LinkedListNode_NewInstance(void* node){
    LinkedListNode* instance=(LinkedListNode*)calloc(1,sizeof(LinkedListNode));

    instance->node=node;

    instance->next=(void*)0;
    instance->last=(void*)0;

    return instance;

}

void LinkedListNode_SetNextNode(LinkedListNode* linkedListNode,void* node){
    LinkedListNode* next=LinkedListNode_NewInstance(node);

    linkedListNode->next=next;

}

void LinkedListNode_SetLastNode(LinkedListNode* linkedListNode,void* node){
    LinkedListNode* last=LinkedListNode_NewInstance(node);

    linkedListNode->last=last;

}

LinkedListNode* LinkedListNode_GetNextNode(LinkedListNode* linkedListNode){
    return linkedListNode->next;

}

LinkedListNode* LinkedListNode_GetLastNode(LinkedListNode* linkedListNode){
    return linkedListNode->last;

}

void LinkedList_Free(LinkedList* linkedList){
    free(linkedList);

}
