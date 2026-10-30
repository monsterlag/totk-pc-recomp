// Camera and viewport sites from MacNativeGames games/totk/runtime/aspect_patch.h and game_patches_v143.c
// @ 933b6a0d1438de160208a0a13eb5e8d7a2cb8094 (MIT, Copyright (c) 2026 MacNativeGames contributors).
// Their runtime veneers become checked, switchable replacements in recomp/patches-v143.txt. The camera keeps its
// vertical field of view and sees more world at the sides. The present still scales the game's canvas to the window;
// vk_exec_draw.c corrects HUD quads separately. The original module text stays unchanged.
#include <string.h>

#include "aspect.h"
#include "cfg.h"
#include "host.h"

static int patch = -1;
static uint32_t ratio_bits;

static void publish(float a) {
    uint32_t bits;
    memcpy(&bits, &a, sizeof bits);
    __atomic_store_n((uint32_t *)((uint8_t *)rt_patch_page() + 56), bits, __ATOMIC_RELAXED);
    __atomic_store_n(&ratio_bits, bits, __ATOMIC_RELAXED);
}

void aspect_init(void) {
    patch = rt_patch_set("aspect");
    if (patch < 0) {
        rt_log("aspect: this build has no camera aspect patches; the original 16:9 mode is used");
        return;
    }
    publish(ASPECT_GAME);
    rt_patch_switch(patch, 0);
}

int aspect_supported(void) { return patch >= 0; }

float aspect_ratio(void) {
    uint32_t bits = __atomic_load_n(&ratio_bits, __ATOMIC_RELAXED);
    float a;
    memcpy(&a, &bits, sizeof a);
    return a > 0 ? a : ASPECT_GAME;
}

void aspect_update(unsigned w, unsigned h) {
    if (patch < 0) return;
    float a = !strcmp(cfg_value("TOTK_ASPECT"), "window") ? aspect_window_ratio(w, h) : ASPECT_GAME;
    int wide = a > ASPECT_GAME * 1.001f;
    if (!wide) a = ASPECT_GAME;
    if (a != aspect_ratio()) {
        publish(a);
        rt_log("aspect: %.6f (%ux%u window), %s", a, w, h, wide ? "ultrawide, centred 16:9 HUD" : "original 16:9");
    }
    rt_patch_switch(patch, wide);
}
