//FLOAT LIST
#include <stdio.h>
#include <unistd.h>

#include <lists.h>

list_node* add_node(list_node *tail, float x, float y, float z)
{
	list_node *new_node = malloc(sizeof(list_node));
	new_node -> x = x;
	new_node -> y = y;
	new_node -> z = z;
	new_node -> next = NULL;

	if(!tail) return new_node;

	tail -> next = new_node;
	return new_node;
}

void list_init(linked_list *list)
{
	list -> count = 0;
	list -> head = NULL;
	list -> tail = NULL;
}

void list_add(linked_list *list, float x, float y, float z)
{
	list -> tail = add_node(list -> tail, x, y, z);
	if(list -> head == NULL) list -> head = list -> tail;
	list -> count ++;
}

list_node* list_get(linked_list *list, int index)
{
	list_node *i;
	for( i = list -> head; i != NULL; i = i -> next)
	{
		if(!index)
		{
			return i;
		}
		index--;
	}
	return NULL;
}

void list_node_free(list_node *node)
{
	if(!node) return;
	list_node_free(node -> next);
	free(node);
}

void list_free(linked_list *list)
{
	if(!list) return;
	list_node_free(list -> head);

}

// INT LIST


int_list_node* int_add_node(int_list_node *tail, int x)
{
	int_list_node *new_node = malloc(sizeof(int_list_node));
	new_node -> x = x;
	new_node -> next = NULL;

	if(!tail) return new_node;

	tail -> next = new_node;
	return new_node;
}

void int_list_init(int_linked_list *list)
{
	list -> count = 0;
	list -> head = NULL;
	list -> tail = NULL;
}

void int_list_add(int_linked_list *list, int x)
{
	list -> tail = int_add_node(list -> tail, x);
	if(list -> head == NULL) list -> head = list -> tail;
	list -> count ++;
}

int_list_node* int_list_get(int_linked_list *list, int index)
{
	int_list_node *i;
	for( i = list -> head; i != NULL; i = i -> next)
	{
		if(!index)
		{
			return i;
		}
		index--;
	}
	return NULL;
}

void int_list_node_free(int_list_node *node)
{
	if(!node) return;
	int_list_node_free(node -> next);
	free(node);
}

void int_list_free(int_linked_list *list)
{
	if(!list) return;
	int_list_node_free(list -> head);

}
