#include "math.h"
#include "ui.h"
#include <raylib.h>

static int fontSize = 16;

void DrawButton(int x, int y, int w, int h, Color color, const char *text,
                VoidAction *action) {
    Vector2 pos = GetMousePosition();

    Rectangle rect = (Rectangle){
        .x = x,
        .y = y,
        .width = w,
        .height = h,
    };

    if (IsMouseButtonPressed(0) && CheckCollisionPointRec(pos, rect)) {
        action();
    }

    DrawRectangleRec(rect, color);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 5);
    Vector2 origin = Center((Rectangle){x, y, w, h}, textSize);

    DrawText(text, origin.x, origin.y, fontSize, BLACK);
}
