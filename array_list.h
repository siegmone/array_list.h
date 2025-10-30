#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "alloc.h"

typedef struct array_list_s {
    void *data;
    allocator_t *allocator;
    size_t cap, size, t_size;
} array_list_t;

void   array_list_init   (array_list_t *list, allocator_t *a, size_t t_size);
void   array_list_deinit (array_list_t *list);
void   array_list_push   (array_list_t *list, const void *ptr);
void   *array_list_get   (array_list_t *list, size_t idx);
size_t array_list_size   (const array_list_t *list);

#endif /* ARRAY_LIST_H */

#ifdef ARRAY_LIST_IMPL

void array_list_init(array_list_t *list, allocator_t *a, size_t t_size) {
    list->data      = NULL;
    list->allocator = a;
    list->cap       = 0;
    list->size      = 0;
    list->t_size    = t_size;
}

void array_list_deinit(array_list_t *list) {
    if (list->data) {
        mem_free(list->allocator, list->data);
        list->data = NULL;
    }
    list->size = list->cap = 0;
}

static void array_list_grow(array_list_t *list, size_t new_cap) {
    if (new_cap <= list->cap) {
        return;
    }

    size_t new_bytes = new_cap * list->t_size;
    void *new_data = mem_alloc(list->allocator, new_bytes);

    if (list->data) {
        memcpy(new_data, list->data, list->size * list->t_size);
        mem_free(list->allocator, list->data);
    }

    list->data = new_data;
    list->cap = new_cap;
}

void array_list_push(array_list_t *list, const void *ptr) {
    if (list->size >= list->cap) {
        array_list_grow(list, list->cap ? list->cap * 2 : 4);
    }

    void *dest = (uint8_t*)list->data + list->size * list->t_size;
    memcpy(dest, ptr, list->t_size);
    list->size++;
}

void *array_list_get(array_list_t *list, size_t idx) {
    return (uint8_t*)list->data + idx* list->t_size;
}

size_t array_list_size(const array_list_t *list) {
    return list->size;
}


#endif /* ARRAY_LIST_IMPL */
