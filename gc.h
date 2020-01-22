
#ifndef _GC_H_
#define _GC_H_

#include <stddef.h>

#include "list.h"

typedef union Reference {
	size_t rnd;
	void  *ptr;
} __attribute__((transparent_union)) ref_t;

typedef struct Allocation {
	size_t usages;
	size_t   size;
	void     *ptr;
	void  *caller;
	ref_t     ref;
	char   orphan;
} alloc_t;

extern list_t *gc_list_root;

extern size_t   gc_incremental(void);
extern size_t   gc_random(void);
extern alloc_t *gc_make(size_t, void *, ref_t);
extern alloc_t *gc_safe(alloc_t **);
extern void     gc_free_ptr(void **);
extern void     gc_free(alloc_t **);
extern void     gc_stats(void);
extern void    *gc_alloc(size_t, void *, ref_t);
extern int      gc_collect_caller_helper(alloc_t *, va_list);
extern void     gc_collect_caller(void *);
extern int      gc_collect_ref_helper(alloc_t *, va_list);
extern void     gc_collect_ref(ref_t);
extern void     gc_collect(void *, ref_t);

#endif
