
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "gc.h"

list_t *gc_list_root = NULL;

size_t gc_random(void) {
	return (size_t) random();
}

size_t gc_incremental(void) {
	static size_t count;
	count += 1;
	return count;
}

alloc_t* gc_make(size_t size, void *caller, ref_t ref) {
	alloc_t *alloc = (alloc_t*) calloc(1, sizeof(alloc_t));
	alloc->usages  = 0x6378;
	alloc->size    = 0;
	alloc->ptr     = calloc(1, size);
	alloc->caller  = caller;
	alloc->ref.ptr = NULL; //gc_incremental();
	alloc->orphan  = 0x00;
	if(alloc->ptr) {
		alloc->size = size;
	}
	if(ref.rnd || ref.ptr) {
		alloc->ref = ref;
	}
	return alloc;
}

alloc_t* gc_safe(alloc_t **alloc) {
	static alloc_t empty;
	memset(&empty, 0, sizeof(empty));
	if(!alloc || !*alloc) return &empty;
	return *alloc;
}

void gc_free_ptr(void **ptr) {
	if(!ptr || !*ptr) return;
	free(*ptr);
	*ptr = NULL;
	return;
}

void gc_free(alloc_t **alloc) {
	//gc_free_ptr(&(gc_safe(alloc)->caller));
	gc_free_ptr(&(gc_safe(alloc)->ptr));
	gc_free_ptr((void**) alloc);
	return;
}

void gc_stats(void) {
	list_stat_t stats = list_get_stats();
	fprintf(
		stderr,
		"\nlist:"
		"\n\tmake:   %lu"
		"\n\tfree:   %lu"
		"\n\teject:  %lu"
		"\n\tinsert: %lu"
		"\n\tremove: %lu"
		"\n",
		stats.make,
		stats.free,
		stats.eject,
		stats.insert,
		stats.remove,
		NULL
	);
	return;
}

void* gc_alloc(size_t size, void *caller, ref_t ref) {
	alloc_t *alloc = gc_make(size, caller, ref);
	list_insert(&gc_list_root, list_make(alloc));
	return alloc->ptr;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcompare-distinct-pointer-types"
#pragma GCC diagnostic ignored "-Wincompatible-function-pointer-types"

int gc_collect_caller_helper(alloc_t *alloc, va_list args) {
	if(!alloc || !args) return 1;
	void *caller = va_arg(args, void*);
	alloc->orphan |= (caller == alloc->caller) ? (1 << 1) : 0;
	return 1;
}

void gc_collect_caller(void *caller) {
	list_each_data(&gc_list_root, &gc_collect_caller_helper, caller);
	return;
}

int gc_collect_ref_helper(alloc_t *alloc, va_list args) {
	if(!alloc || !args) return 1;
	ref_t ref = va_arg(args, ref_t);
	alloc->orphan |= memcmp(&ref, &alloc->ref, sizeof(ref_t)) ? 0 : (1 << 2);
	return 1;
}

void gc_collect_ref(ref_t ref) {
	list_each_data(&gc_list_root, &gc_collect_ref_helper, ref);
	return;
}

void gc_collect(void *caller, ref_t ref) {
	if(ref.ptr || ref.rnd)
		gc_collect_ref(ref);
	if(caller)
		gc_collect_caller(caller);
	list_t *node = gc_list_root;
	while(node && node->next != gc_list_root) {
		alloc_t *alloc = (alloc_t*) node->data;
		//void hexdump(void*, size_t);
		//hexdump(node, sizeof(list_t));
		//hexdump(alloc, sizeof(alloc_t));
		//hexdump(alloc->ptr, alloc->size);
		list_t *next = node->next;
		if(alloc->orphan) {
			list_t *orphan = list_remove(&gc_list_root, node);
			assert(orphan);
			assert(node == orphan);
			assert(alloc == orphan->data);
			gc_free_ptr(&alloc->ptr);
			gc_free((alloc_t**) &orphan->data);
			list_free_ptr((void**) &orphan);
		} else printf("! ORPHAN: node:%p alloc:%p \n", node, alloc);
		node = next;
		continue;
	}
	return;
}

/*
void gc_collect_like(void *ptr) {
	return;
}
*/

#pragma GCC diagnostic pop
