#include <assert.h>
#include <stdlib.h>

#include "arena.h"

Region *InitRegion(unsigned long size);
void DeinitRegion(Region *r);

void InitArena(Arena *a) {
}

void DeinitArena(Arena *a) {
}

void *ArenaAlloc(Arena *a, unsigned long size) {
    Region *region = InitRegion(size);

    if (a->tail == NULL) {
        assert(a->head == NULL);

        a->tail = region;
        a->head = a->tail;
    } else {
        a->tail->next = region;
        a->tail = region;
    }

    return region->data;
}

void ArenaFree(Arena *a) {
    if (a->tail == NULL) {
        assert(a->head == NULL);
        return;
    }

    DeinitRegion(a->head);

    a->head = NULL;
    a->tail = NULL;
}

// Private methods

Region *InitRegion(unsigned long size) {
    Region *r = malloc(size + sizeof(Region));

    r->data = malloc(size);

    return r;
}

void DeinitRegion(Region *r) {
    Region *cur = r;

    while (cur != NULL) {
        Region *n = cur->next;

        free(cur->data);
        free(cur);

        cur = n;
    }
}
