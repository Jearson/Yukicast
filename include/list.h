#ifndef LIST_H
#define LIST_H

/**
 * Doubly linked list implementation.
 */

typedef struct list_node {
	long int id;
	void *data;
	struct list_node *next;
	struct list_node *prev;
} list_node_t;

typedef struct list {
	int size;
	list_node_t *head;
	list_node_t *tail;
} list_t;

void list_init(list_t *list);
int list_size(list_t *list);
void list_push_head(void *data, long int id);
void list_push_tail(void *data, long int id);
list_node_t *list_pop_head(list_t *list);
list_node_t *list_pop_tail(list_t *list);
list_node_t *list_remove(long int id);
list_node_t *list_get(long int id);

#endif /* LIST_H */
