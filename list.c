
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "list.h"

static list_stat_t list_stats;

list_t* list_make(void *data) {
	list_t *list = (list_t*) calloc(1, sizeof(list_t));
	list_stats.make += 1;
	list->next = NULL;
	list->data = data;
	return list;
}

list_t* list_prev(list_t **list, list_t *node) {
	if(!list) return NULL;
	for(list_t *tmp=*list; tmp && tmp->next != *list; tmp=tmp->next) {
		if(node == tmp->next) return tmp;
		continue;
	}
	return NULL;
}

void list_insert(list_t **list, list_t *node) {
	if(!list || !node) return;
	list_stats.insert += 1;
	node->next = *list;
	*list      =  node;
	return;
}


list_t* list_eject(list_t **list) {
	if(!list || !*list) return NULL;
	list_stats.eject += 1;
	list_t *node =  *list;
	*list        = (*list)->next;
	return node;
}

list_t* list_remove(list_t **list, list_t *node) {
	if(!list || !node) return NULL;
	list_stats.remove += 1;
	if(node == *list) return list_eject(list);
	list_t *prev = list_prev(list, node);
	if(prev) return list_eject(&(prev->next));
	return NULL;
}

list_t* list_safe(list_t **list) {
	static list_t empty;
	memset(&empty, 0, sizeof(empty));
	if(!list || !*list) {
		list_stats.safe += 1;
		return &empty;
	}
	return *list;
}

void list_free_ptr(void **ptr) {
	if(!ptr || !*ptr) return;
	list_stats.free += 1;
	free(*ptr);
	*ptr = NULL;
	return;
}

void list_free(list_t **node) {
	if(!node || !*node) return;
	list_t *next = (*node)->next;
	//list_free_ptr(&((*node)->data));
	list_free_ptr((void**) node);
	list_free(&next);
	return;
}

list_t* list_tail(list_t **list) {
	if(!list || !*list) return  NULL;
	list_stats.tail += 1;
	if(!(*list)->next)  return *list;
	return list_tail(&((*list)->next));
}

list_t* list_find(list_t **list, void *data) {
	if(!list) return NULL;
	for(list_t *node=*list; node && node->next != *list; node=node->next) {
		if(data == node->data) {
			return node;
		}
		continue;
	}
	return NULL;
}

void list_each(list_t **list, list_each_f function, ...) {
	va_list args;
	va_start(args, function);
	for(
		list_t *node=*list;
		node && node->next != *list;
		node=node->next
	) {
		va_list params;
		va_copy(params, args);
		function(node, params);
		va_end(params);
	}
	va_end(args);
	return;
}

void list_each_data(list_t **list, list_each_data_f function, ...) {
	va_list args;
	va_start(args, function);
	for(
		list_t *node=*list;
		node && node->next != *list;
		node=node->next
	) {
		va_list params;
		va_copy(params, args);
		function(node->data, params);
		va_end(params);
	}
	va_end(args);
	return;
}

list_stat_t list_get_stats(void) {
	return list_stats;
}
