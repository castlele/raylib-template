#include "screen.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define DEFAULT_WIN_W 800
#define DEFAULT_WIN_H 600
#define WIN_NAME "Template Project"

static void UpdateDrawFrame();
static void Update();
static void Draw();

static void InitScreen(ScreenType);
static void DeinitScreen(ScreenType);
static void UpdateScreen(ScreenType, float dt);
static void DrawScreen(ScreenType);

static void NavigationStarted(ScreenType from, ScreenType to);
static void NavigationFinished(ScreenType from, ScreenType to);

static ScreenType currentScreen = MAIN;

int main() {
    InitWindow(DEFAULT_WIN_W, DEFAULT_WIN_H, WIN_NAME);

    InitScreen(currentScreen);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    DeinitScreen(currentScreen);

    CloseWindow();

    return 0;
}

static void UpdateDrawFrame() {
    Update();
    Draw();
}

static void Update() {
    float dt = GetFrameTime();

    UpdateScreen(currentScreen, dt);
}

static void Draw() {
    ClearBackground(RAYWHITE);

    BeginDrawing();
    DrawScreen(currentScreen);
    EndDrawing();
}

static void InitScreen(ScreenType screen) {
    switch (screen) {
    case NONE:
        break;
    case MAIN:
        InitMainScreen();
        break;
    case SETTINGS:
        InitSettingsScreen();
        break;
    }
}

static void DeinitScreen(ScreenType screen) {
    switch (screen) {
    case NONE:
        break;
    case MAIN:
        DeinitMainScreen();
        break;
    case SETTINGS:
        DeinitSettingsScreen();
        break;
    }
}

static void UpdateScreen(ScreenType screen, float dt) {
    if (IsInTransition()) {
        UpdateTransition();
        return;
    }

    switch (screen) {
    case NONE:
        break;
    case MAIN:
        UpdateMainScreen(dt);

        if (IsMainFinished()) {
            ScreenType to = MainNavigateToScreen();

            NavigateToScreen(MAIN, to, FADEIN_FADEOUT, NavigationStarted,
                             NavigationFinished);
        }
        break;
    case SETTINGS:
        UpdateSettingsScreen();

        if (IsSettingsFinished()) {
            ScreenType to = SettingsNavigateToScreen();

            NavigateToScreen(SETTINGS, to, FADEIN_FADEOUT, NavigationStarted,
                             NavigationFinished);
        }
        break;
    }
}

static void DrawScreen(ScreenType screen) {
    switch (screen) {
    case NONE:
        break;
    case MAIN:
        DrawMainScreen();
        break;
    case SETTINGS:
        DrawSettingsScreen();
        break;
    }

    if (IsInTransition()) {
        DrawTransition();
    }
}

static void NavigationStarted(ScreenType from, ScreenType to) {
    currentScreen = to;
    InitScreen(to);
}

static void NavigationFinished(ScreenType from, ScreenType to) {
    DeinitScreen(from);
}
