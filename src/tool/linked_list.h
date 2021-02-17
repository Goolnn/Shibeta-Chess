#ifndef SHIBETA_CHESS_TOOL_LINKED_LIST
#define SHIBETA_CHESS_TOOL_LINKED_LIST

struct LinkedList;
struct LinkedListNode;

typedef struct LinkedList LinkedList;
typedef struct LinkedListNode LinkedListNode;

//LinkedList
LinkedList* linked_list_new(int,void*[]);

void linked_list_add_to_head(LinkedList*,void*);
void linked_list_add_to_tail(LinkedList*,void*);

void linked_list_add_to(LinkedList*,int,void*);

void linked_list_remove_head(LinkedList*);
void linked_list_remove_tail(LinkedList*);

void linked_list_remove(LinkedList*,int);

LinkedListNode* linked_list_get_head(LinkedList*);
LinkedListNode* linked_list_get_tail(LinkedList*);

LinkedListNode* linked_list_get(LinkedList*,int);

int linked_list_size(LinkedList*);

void linked_list_free(LinkedList*);

//LinkedListNode
void* linked_list_node_get_pointer(LinkedListNode*);

void* linked_list_node_get_next(LinkedListNode*);
void* linked_list_node_get_preview(LinkedListNode*);

#endif
