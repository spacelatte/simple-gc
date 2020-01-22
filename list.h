

#ifndef _LIST_H_
#define _LIST_H_

#include <stddef.h>

typedef struct List {
	struct List *next;
	unsigned char *data;
} list_t;

typedef struct ListStat {
	size_t make;
	size_t free;
	size_t safe;
	size_t tail;
	size_t insert;
	size_t remove;
	size_t eject;
} list_stat_t;

typedef int (*list_each_f) (list_t*, va_list);
typedef int (*list_each_data_f) (void*, va_list);

extern list_t *list_make(void *);
extern list_t *list_prev(list_t **, list_t *);
extern void    list_insert(list_t **, list_t *);
extern list_t *list_eject(list_t **);
extern list_t *list_remove(list_t **, list_t *);
extern list_t *list_safe(list_t **);
extern void    list_free_ptr(void **);
extern void    list_free(list_t **);
extern list_t *list_tail(list_t **);
extern list_t *list_find(list_t **, void *);
extern void    list_each(list_t **, list_each_f, ...);
extern void    list_each_data(list_t **, list_each_data_f, ...);
extern list_stat_t list_get_stats(void);

#endif
