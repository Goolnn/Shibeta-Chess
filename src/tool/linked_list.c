#include <stdlib.h>

#include "linked_list.h"

LinkedListNode* linked_list_node_new(void*,void*,void*);

void linked_list_node_free(LinkedListNode*);

struct LinkedList{
    int size;

    LinkedListNode* head;
    LinkedListNode* tail;

};

struct LinkedListNode{
    void* pointer;

    void* preview;
    void* next;

};

//LinkedList
LinkedList* linked_list_new(int size,void* pointers[]){
    LinkedList* instance=calloc(1,sizeof(LinkedList));

    instance->size=size;

    if(size!=0&&pointers!=NULL){
        LinkedListNode* nodes[size];

        //创建链表节点数组
        for(int i=0;i<size;i++){
            nodes[i]=linked_list_node_new(pointers[i],NULL,NULL);
            
        }

        //连接链表节点数组
        for(int i=0;i<size;i++){
            if(i+1!=size){
                nodes[i]->next=nodes[i+1];
                nodes[i+1]->preview=nodes[i];

            }

        }

        //设定头尾节点
        instance->head=nodes[0];
        instance->tail=nodes[size-1];

    }

    return instance;

}

void linked_list_add_to_head(LinkedList* list,void* pointer){
    LinkedListNode* head=linked_list_get_head(list);
    LinkedListNode* node;

    if(head!=NULL){
        node=linked_list_node_new(pointer,NULL,head);
        
        head->preview=node;

        list->head=node;

    }else{
        node=linked_list_node_new(pointer,NULL,NULL);

        list->head=node;

    }

    list->size++;

}

void linked_list_add_to_tail(LinkedList* list,void* pointer){
    LinkedListNode* tail=linked_list_get_tail(list);
    LinkedListNode* node;

    if(tail!=NULL){
        node=linked_list_node_new(pointer,tail,NULL);
        
        tail->next=node;

        list->tail=node;

    }else{
        node=linked_list_node_new(pointer,NULL,NULL);

        list->head=node;
        list->tail=node;

    }

    list->size++;

}

void linked_list_add_to(LinkedList* list,int index,void* pointer){
    if(index==0){
        linked_list_add_to_head(list,pointer);

    }else if(index==linked_list_size(list)-1){
        linked_list_add_to_tail(list,pointer);

    }else{
        LinkedListNode* node=linked_list_node_new(pointer,linked_list_get(list,index-1),linked_list_get(list,index));

        linked_list_get(list,index)->preview=node;
        linked_list_get(list,index-1)->next=node;

    }

    list->size++;

}

void linked_list_remove_head(LinkedList* list){
    if(linked_list_get_head(list)!=NULL){
        if(linked_list_size(list)>=2){
            LinkedListNode* old_head=linked_list_get_head(list);
            LinkedListNode* node=linked_list_get(list,1);

            node->preview=NULL;

            list->head=node;

            linked_list_node_free(old_head);

        }else{
            list->head=NULL;
            list->tail=NULL;

            linked_list_node_free(linked_list_get_head(list));

        }

        list->size--;

    }

}

void linked_list_remove_tail(LinkedList* list){
    if(linked_list_get_tail(list)!=NULL){
        if(linked_list_size(list)>=2){
            LinkedListNode* old_tail=linked_list_get_tail(list);
            LinkedListNode* node=linked_list_get(list,linked_list_size(list)-2);

            node->next=NULL;

            list->tail=node;

            linked_list_node_free(old_tail);

        }else{
            list->head=NULL;
            list->tail=NULL;

            linked_list_node_free(linked_list_get_tail(list));

        }

    }

    list->size--;

}

void linked_list_remove(LinkedList* list,int index){
    if(index==0){
        linked_list_remove_head(list);

    }else if(index==linked_list_size(list)-1){
        linked_list_remove_tail(list);

    }else{
        LinkedListNode* old_node=linked_list_get(list,index);

        linked_list_get(list,index-1)->next=linked_list_get(list,index+1);
        linked_list_get(list,index)->preview=linked_list_get(list,index-1);

        linked_list_node_free(old_node);

    }

    list->size--;

}

LinkedListNode* linked_list_get_head(LinkedList* list){
    return list->head;

}

LinkedListNode* linked_list_get_tail(LinkedList* list){
    return list->tail;

}

LinkedListNode* linked_list_get(LinkedList* list,int index){
    LinkedListNode* node=linked_list_get_head(list);

    int i=0;

    while(i!=index){
        node=node->next;

        i++;

    }

    return node;

}

int linked_list_size(LinkedList* list){
    return list->size;

}

void linked_list_free(LinkedList* list){
    LinkedListNode* node=linked_list_get_head(list);

    while(node!=NULL){
        linked_list_node_free(node);

        node=node->next;

    }

    free(list);

}

//LinkedListNode
LinkedListNode* linked_list_node_new(void* pointer,void* preview,void* next){
    LinkedListNode* instance=calloc(1,sizeof(LinkedListNode));

    instance->pointer=pointer;

    instance->preview=preview;
    instance->next=next;

    return instance;

}

void* linked_list_node_get_pointer(LinkedListNode* node){
    return node->pointer;

}

void* linked_list_node_get_next(LinkedListNode* node){
    return node->next;

}

void* linked_list_node_get_preview(LinkedListNode* node){
    return node->preview;

}

void linked_list_node_free(LinkedListNode* node){
    free(node);

}
