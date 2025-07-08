#include "math.h"

Vector2 Center(Rectangle parent, Vector2 child) {
    Vector2 origin = {
        .x = parent.x + (parent.width - child.x) / 2,
        .y = parent.y + (parent.height - child.y) / 2,
    };

    return origin;
}

Vector2 GetCenterScreen() {
    return (Vector2){.x = GetScreenWidth(), .y = GetScreenHeight()};
}
