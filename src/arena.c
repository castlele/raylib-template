#include <assert.h>
#include <stdlib.h>

#include "arena.h"

Region *InitRegion(unsigned long size);
void DeinitRegion(Region *r);

void InitArena(Arena *a) {
    a->head = NULL;
    a->tail = NULL;
}

void DeinitArena(Arena *a) {
    ArenaFree(a);
}

void *ArenaAlloc(Arena *a, unsigned long size) {
    Region *region = InitRegion(size);

    if (a->tail == NULL) {
        // assert(a->head == NULL);

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

    Region *r = a->head;

    while (r) {
        Region *tmp = r;

        r = r->next;

        DeinitRegion(tmp);
    }

    a->head = NULL;
    a->tail = NULL;
}

// Private methods

Region *InitRegion(unsigned long size) {
    Region *r = malloc(size + sizeof(Region));

    if (!r) return NULL;

    r->next = NULL;
    r->data = r + 1;

    return r;
}

void DeinitRegion(Region *r) {
    free(r);
}
