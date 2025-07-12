#include <math.h>
#include <raylib.h>

#include "screen.h"
#include "types.h"

static void InitNavigation();
static void DeinitNavigation();

static void InitFadeNavigation();
static void UpdateFadeTransition();
static void DrawFadeTransition();

static NavigationType _type;
static ScreenType _from;
static ScreenType _to;
static ScreensNavigationAction *_onStart;
static ScreensNavigationAction *_onFinish;
static BOOL transition = NO;

void NavigateToScreen(
    ScreenType from,
    ScreenType to,
    NavigationType type,
    ScreensNavigationAction *onStart,
    ScreensNavigationAction *onFinish
) {
    _type = type;
    _from = from;
    _to = to;
    _onStart = onStart;
    _onFinish = onFinish;

    InitNavigation();
}

void UpdateTransition() {
    switch (_type) {
        case FADEIN_FADEOUT:
            UpdateFadeTransition();
            break;
    }
}

void DrawTransition() {
    switch (_type) {
        case FADEIN_FADEOUT:
            DrawFadeTransition();
            break;
    }
}

BOOL IsInTransition() {
    return transition;
}

// Private methods

static void InitNavigation() {
    switch (_type) {
        case FADEIN_FADEOUT:
            InitFadeNavigation();
            break;
    }

    transition = YES;
    _onStart(_from, _to);
}

// value from 0.0 to PI
static float progress = 0.0;
static float fade = 0.0;
static float step = 0.1;

static void InitFadeNavigation() {
    fade = 0.0;
    progress = 0.0;
}

static void DeinitNavigation() {
    _from = NONE;
    _to = NONE;
    _onStart = 0;
    _onFinish = 0;
}

static void UpdateFadeTransition() {
    fade = sin(progress);
    progress += step;

    if (progress >= PI) {
        transition = NO;
        _onFinish(_from, _to);
        DeinitNavigation();
    }
}

static void DrawFadeTransition() {
    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Fade(BLACK, fade)
    );
}
