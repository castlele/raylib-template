#include <raylib.h>
#include <string.h>

#include "arena.h"
#include "math.h"
#include "physics.h"
#include "screen.h"
#include "types.h"
#include "ui.h"

#define MAX_PARTICLES 5

typedef struct Particle {
    Color color;
    int size;
    Vector2 v;
    Collider *c;
} Particle;

static PWorld *w;

static void NavigateSettings();
static BOOL MainCollisionRule(Collider *lhs, Collider *rhs);

static Arena mainArena;
static Particle *particles;
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
    InitArena(&mainArena);

    w = InitPWorldOnArena(&mainArena, (Vector2){.x = 0, .y = 150});
    particles = ArenaAlloc(&mainArena, MAX_PARTICLES * sizeof(Particle));

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle p = {
            .color = {.r = 255, .g = 255 - 50 * i, .b = 255, .a = 255},
            .size = 50,
            .v =
                {
                    .x = GetRandomValue(50, GetScreenWidth() - 50),
                    .y = GetRandomValue(50, GetScreenHeight() - 50),
                },
        };

        p.c = InitColliderInWorld(w, p.v, p.size, p.size, ColliderTypeDynamic,
                                  50);
        SetColliderMask(p.c, "Particle");
        particles[i] = p;
    }

    SetCollisionRule(w, MainCollisionRule);

    InitColliderInWorld(w, (Vector2){.x = 0, .y = GetScreenHeight()},
                        GetScreenWidth(), 10, ColliderTypeStatic, 0);
}

void DeinitMainScreen() {
    navigateTo = NONE;
    DeinitPWorldOnArena(w);
    DeinitArena(&mainArena);
    particles = 0;
    w = NULL;
}

void UpdateMainScreen(float dt) {
    UpdatePWorld(w, dt);
    screenRect.width = GetScreenWidth();
    screenRect.height = GetScreenHeight();
}

void DrawMainScreen() {
    DrawRectangle(0, 0, screenRect.width, screenRect.height, WHITE);

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle p = particles[i];
        DrawCircle(p.c->pos.x, p.c->pos.y, p.size, p.color);
    }

    Vector2 buttonOrigin = Center(screenRect, buttonSize);
    DrawButton(buttonOrigin.x, buttonOrigin.y, buttonSize.x, buttonSize.y, BLUE,
               "Settings", NavigateSettings);
}

BOOL IsMainFinished() { return navigateTo != NONE; }

ScreenType MainNavigateToScreen() { return navigateTo; }

// Private methods

static void NavigateSettings() { navigateTo = SETTINGS; }

static BOOL MainCollisionRule(Collider *lhs, Collider *rhs) {
    if (strcmp(lhs->mask, rhs->mask) == 0) {
        return NO;
    }

    return YES;
}
