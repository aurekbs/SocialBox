#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * Le paramètre \p data correspond à la donnée à affecter au nouveau nœud.
 * 
 * @param[in] data
 * @return le nouveau nœud créé
 */
static struct list_node_t * new_list_node(void * data) {
	struct list_node_t * node = malloc(sizeof(struct list_node_t));
	node->data = data;
	node->predecessor = NULL;
	node->successor = NULL;
	return node;
}

void * get_list_node_data(const struct list_node_t * node) {
	assert(node);
	return node->data;
}

struct list_node_t * get_successor(const struct list_node_t* node) {
	assert(node);
	return node->successor;
}

struct list_node_t * get_predecessor(const struct list_node_t * node) {
	assert(node);
	return node->predecessor;
}

void set_list_node_data(struct list_node_t * node, void * new_data) {
	assert(node);
	node->data = new_data;
}

void set_successor(struct list_node_t * node, struct list_node_t * new_successor) {
	assert(node);
	node->successor = new_successor;
}

void set_predecessor(struct list_node_t * node, struct list_node_t * new_predecessor) {
	assert(node);
	node->predecessor = new_predecessor;
}

/********************************************************************
 * list_t (Double-Linked List)
 ********************************************************************/

struct list_t * new_list() {
	struct list_t * list = malloc(sizeof(struct list_t));
	list->size=0;
	return list;
}

int list_is_empty(const struct list_t * L) {
	assert(L);
	if(L->head == NULL) {
		return 1;
	} else { return 0;}
}

unsigned int get_list_size(const struct list_t * L) {
	assert(L);
	return L->size;
}

struct list_node_t * get_list_head(const struct list_t * L) {
	assert(L);
	return L->head;
}

struct list_node_t * get_list_tail(const struct list_t * L) {
	assert(L);
	return L->tail;
}

void increase_list_size(struct list_t * L) {
	assert(L);
	L->size++;
}

void decrease_list_size(struct list_t * L) {
	assert(L);
	L->size--;
}

void set_list_size(struct list_t * L, unsigned int new_size) {
	assert(L);
	L->size = new_size;
}

void set_list_head(struct list_t * L, struct list_node_t * new_head) {
	assert(L);
	L->head = new_head;
}

void set_list_tail(struct list_t * L, struct list_node_t * new_tail) {
	assert(L);
	L->tail = new_tail;
}

void delete_list(struct list_t * L, void (*freeData)(void *)) {
	assert(L);

		struct list_node_t *curr = L->head;
		struct list_node_t *succ;
		for(int i = 0; i < get_list_size(L); ++i) {
			succ = curr->successor;
			freeData(curr->data);
			free(curr);
			curr = succ;
		}

	L->size = 0;
}

void view_list(const struct list_t * L, void (*viewData)(const void *)) {
	assert(L);
	struct list_node_t * curr = get_list_head(L);
	printf("liste :[ ");
	while(curr) {
		viewData(curr->data);
		printf(" ");
		curr = get_successor(curr);
	}
	printf("]\n");
}
void list_insert_first(struct list_t * L, void * data) {
	assert(L);
	struct list_node_t * new_node = new_list_node(data);
	if(list_is_empty(L)) {
		set_list_head(L,new_node);
		set_list_tail(L,new_node);
	}
	else {
		set_predecessor(get_list_head(L),new_node);
		set_successor(new_node,get_list_head(L));
		set_list_head(L,new_node);
	}
	increase_list_size(L);

}

void list_insert_last(struct list_t * L, void * data) {
	assert(L);
	struct list_node_t * new_node = new_list_node(data);
	if(list_is_empty(L)) {
		set_list_tail(L,new_node);
		set_list_head(L,new_node);
	}
	else {
		struct list_node_t *tail = get_list_tail(L);
		set_successor(tail,new_node);
		set_predecessor(new_node,tail);
		set_list_tail(L,new_node);

	}
	increase_list_size(L);
}
void list_insert_after(struct list_t * L, void * data, struct list_node_t * node) { //pas sûr
	assert(L);
	assert(node);
	struct list_node_t * new_node = new_list_node(data);
	if(get_list_size(L)==1) {
		set_successor(get_list_head(L),new_node);
		set_predecessor(new_node,get_list_head(L));
		set_list_tail(L,new_node);
	}
	else {
		set_successor(new_node,get_successor(node));
		set_predecessor(get_successor(node),new_node);
		set_successor(node,new_node);
		set_predecessor(new_node,node);

	}
	increase_list_size(L);
}

void * list_remove_first(struct list_t * L) { //pas sûr
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	if(!list_is_empty(L)) {
		struct list_node_t * curr = get_list_head(L);
		void * data=get_list_node_data(curr);
		set_list_head(L,get_successor(curr));
		set_predecessor(get_list_head(L),NULL);
		set_successor(curr,NULL);
		free(curr);
		decrease_list_size(L);
		return data;

	}
	return NULL;



}

void * list_remove_last(struct list_t * L) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	if(!list_is_empty(L)) {
		struct list_node_t * curr = get_list_tail(L);
		set_list_tail(L,get_predecessor(curr));
		set_successor(get_list_tail(L),NULL);
		set_predecessor(curr,NULL);
		void * data=get_list_node_data(curr);
		free(curr);
		decrease_list_size(L);
		return data;
	}
	return NULL;

}

void * list_remove_node(struct list_t * L, struct list_node_t * node) {
	assert(L);
	assert(get_list_head(L) && get_list_tail(L));
	if(!list_is_empty(L)){
	struct list_node_t * succ = get_predecessor(node);
	set_successor(get_predecessor(node),get_successor(node));
	set_predecessor(get_successor(node),succ);
	set_predecessor(node,NULL);
	set_successor(node,NULL);
	void * data=get_list_node_data(node);
	free(node);
	decrease_list_size(L);
	return data;
	}
	return NULL;
}
struct list_node_t * list_find_node(struct list_t * L, void * data) {
	assert(L);
	struct list_node_t * curr = get_list_head(L);
	while(curr) {
		if(get_list_node_data(curr) == data) {
			return curr;
		}
		else{
				curr=get_successor(curr);
		}
	}
	return NULL;
}
void list_swap_nodes_data(struct list_node_t * node1, struct list_node_t * node2) {
	assert(node1);
	assert(node2);
	void * temp=get_list_node_data(node1);
	set_list_node_data(node1,get_list_node_data(node2));
	set_list_node_data(node2,temp);
}