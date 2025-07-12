#include <raylib.h>

#include "screen.h"
#include "types.h"
#include "ui.h"

static void NavigateBackToMain();

static ScreenType navigateTo = NONE;

void InitSettingsScreen() {
}

void DeinitSettingsScreen() {
    navigateTo = NONE;
}

void UpdateSettingsScreen() {
}

void DrawSettingsScreen() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);

    DrawButton(50, 50, 100, 50, BLUE, "Back", NavigateBackToMain);
}

BOOL IsSettingsFinished() {
    return navigateTo != NONE;
}

ScreenType SettingsNavigateToScreen() {
    return navigateTo;
}

// Private methods

static void NavigateBackToMain() {
    navigateTo = MAIN;
}
