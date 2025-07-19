#ifndef ARENA_H_
#define ARENA_H_

typedef struct Region Region;

// TODO: Recreate with Tsoding's arena (store data[]): https://github.com/tsoding/arena
struct Region {
    Region *next;
    void *data;
};

typedef struct Arena {
    Region *head, *tail;
} Arena;

void InitArena(Arena *a);
void DeinitArena(Arena *a);

void *ArenaAlloc(Arena *a, unsigned long size);
void ArenaFree(Arena *a);

#endif // ARENA_H_
