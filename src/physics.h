#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <raylib.h>
#include <raymath.h>

#include "arena.h"
#include "types.h"

#define MAX_COLLIDES 1000
#define COLLIDER_MASK_SIZE 16
#define MAX_RULES 32

typedef enum ColliderType {
    ColliderTypeStatic,
    ColliderTypeDynamic,
} ColliderType;

typedef enum CollisionSide {
    CollisionSideTop,
    CollisionSideBottom,
    CollisionSideLeft,
    CollisionSideRight,
} CollisionSide;

typedef struct Collider {
    Vector2 pos;
    Vector2 vel;
    Vector2 force;
    ColliderType type;
    char mask[COLLIDER_MASK_SIZE];
    float width;
    float height;
    float mass;
    BOOL active;
} Collider;

void SetColliderMask(Collider *c, char *mask);
BOOL CheckCollision(Collider *lhs, Collider *rhs);
CollisionSide GetCollisionSide(bool isCollided, Collider *lhs, Collider *rhs);

typedef BOOL(CollisionRule)(Collider *lhs, Collider *rhs);

typedef struct PWorld {
    Vector2 gravity;
    Collider **colliders;
    CollisionRule *rules[MAX_RULES];
    Arena *arena;
    int topCollider;
    int maxCollidersCapacity;
    int topRule;
} PWorld;

PWorld *InitPWorld(Vector2 gravity);
PWorld *InitPWorldOnArena(Arena *a, Vector2 gravity);

Collider *InitColliderInWorld(PWorld *w, Vector2 pos, float width, float height,
                              ColliderType type, float mass);

void SetCollisionRule(PWorld *w, CollisionRule *rule);

void UpdatePWorld(PWorld *w, float dt);
void DrawPWorld(PWorld *w);

void DeinitPWorld(PWorld *w);
void DeinitPWorldOnArena(PWorld *w);

#endif // PHYSICS_H_
