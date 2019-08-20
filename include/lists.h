#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

typedef struct _list_node
{
	float x, y, z;
	struct _list_node *next;
}list_node;

typedef struct _linked_list
{
	list_node *head, *tail;
	int count;
}linked_list;

list_node* add_node(list_node *tail, float x, float y, float z);
list_node* list_get(linked_list *list, int index);
void list_init(linked_list *list);
void list_add(linked_list *list, float x, float y, float z);
void list_node_free(list_node *node);
void list_free(linked_list *list);



typedef struct _int_list_node
{
	int x;
	struct _int_list_node *next;
}int_list_node;

typedef struct _int_linked_list
{
	int_list_node *head, *tail;
	int count;
}int_linked_list;

int_list_node* int_add_node(int_list_node *tail, int x);
void int_list_init(int_linked_list *list);
void int_list_add(int_linked_list *list, int x);
int_list_node* int_list_get(int_linked_list *list, int index);
void int_list_node_free(int_list_node *node);
void int_list_free(int_linked_list *list);

#endif // LISTS_H_INCLUDED
