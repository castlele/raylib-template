#include <assert.h>
#include <string.h>

#include "physics.h"

void SetColliderMask(Collider *c, char *mask) {
    strcpy(c->mask, mask);
}

BOOL CheckCollision(Collider *lhs, Collider *rhs) {
    if (!lhs->active || !rhs->active) {
        return false;
    }

    float pmaxX = fmax(lhs->pos.x, rhs->pos.x);
    float pmaxY = fmax(lhs->pos.y, rhs->pos.y);
    float pminX = fmin(lhs->pos.x + lhs->width, rhs->pos.x + rhs->width);
    float pminY = fmin(lhs->pos.y + lhs->height, rhs->pos.y + rhs->height);

    return pminX - pmaxX >= 0 && pminY - pmaxY >= 0;
}

CollisionSide GetCollisionSide(bool isCollided, Collider *lhs, Collider *rhs) {
    assert(isCollided);

    float right = (lhs->pos.x + lhs->width) - rhs->pos.x;
    float left = (rhs->pos.x + rhs->width) - lhs->pos.x;
    float top = (rhs->pos.y + rhs->height) - lhs->pos.y;
    float bottom = (lhs->pos.y + lhs->height) - rhs->pos.y;

    if (right <= left && right <= top && right <= bottom) {
        return CollisionSideRight;
    } else if (left <= top && left <= bottom) {
        return CollisionSideLeft;
    } else if (top <= bottom) {
        return CollisionSideTop;
    } else {
        return CollisionSideBottom;
    }
}
