#include <raylib.h>

#include "screen.h"
#include "types.h"
#include "ui.h"
#include "math.h"

static void NavigateSettings();

static ScreenType navigateTo = NONE;
static Rectangle screenRect = {
    .x = 0,
    .y = 0,
};
static Vector2 buttonSize = {
    .x = 100,
    .y = 50,
};

void InitMainScreen() {
}

void DeinitMainScreen() {
    navigateTo = NONE;
}

void UpdateMainScreen() {
    screenRect.width = GetScreenWidth();
    screenRect.height = GetScreenHeight();
}

void DrawMainScreen() {
    DrawRectangle(0, 0, screenRect.width, screenRect.height, RED);

    Vector2 buttonOrigin = Center(screenRect, buttonSize);
    DrawButton(
        buttonOrigin.x,
        buttonOrigin.y,
        buttonSize.x,
        buttonSize.y,
        BLUE,
        "Settings",
        NavigateSettings
    );
}

BOOL IsMainFinished() {
    return navigateTo != NONE;
}

ScreenType MainNavigateToScreen() {
    return navigateTo;
}

// Private methods

static void NavigateSettings() {
    navigateTo = SETTINGS;
}

