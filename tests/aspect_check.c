// No game files or GPU: exercise the live window-aspect setting, its fallback and the HUD rectangle arithmetic.
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "aspect.h"

static _Alignas(16) unsigned char page[4096];
static const char *setting = "16:9";
static int supported = 0, enabled;
void *rt_patch_page(void) { return page; }
int rt_patch_set(const char *name) { assert(!strcmp(name, "aspect")); return supported ? 0 : -1; }
void rt_patch_switch(int set, int on) { assert(set == 0); enabled = on; }
const char *cfg_value(const char *key) { assert(!strcmp(key, "TOTK_ASPECT")); return setting; }
void rt_log(const char *fmt, ...) { (void)fmt; }

static void close_to(float a, float b) { assert(fabsf(a - b) < 0.0001f); }

int main(void) {
    aspect_init();
    assert(!aspect_supported());
    setting = "window";
    aspect_update(3440, 1440);
    assert(!enabled);
    close_to(aspect_ratio(), ASPECT_GAME);
    supported = 1;
    aspect_init();
    assert(aspect_supported() && !enabled);
    for (unsigned i = 0; i < 3; i++) {
        unsigned w[] = {2560, 3440, 5120}, h[] = {1080, 1440, 1440};
        aspect_update(w[i], h[i]);
        assert(enabled);
        float a, want = (float)w[i] / h[i];
        memcpy(&a, page + 56, 4);
        close_to(a, want);
        close_to(aspect_ratio(), want);
        float x = 0, size = (float)w[i], k = want / ASPECT_GAME;
        aspect_ui_span(w[i] * 0.5f, k, &x, &size);
        close_to(x, (w[i] - h[i] * ASPECT_GAME) * 0.5f);
        close_to(size / h[i], ASPECT_GAME);
        // A clipped right half remains the right half of the transformed HUD.
        x = w[i] * 0.5f, size = w[i] * 0.5f;
        aspect_ui_span(w[i] * 0.5f, k, &x, &size);
        close_to(x, w[i] * 0.5f);
        close_to(size, h[i] * ASPECT_GAME * 0.5f);
    }
    aspect_update(1920, 1080);   // resizing back restores the original camera and HUD
    assert(!enabled);
    close_to(aspect_ratio(), ASPECT_GAME);
    close_to(aspect_window_ratio(1920, 1200), ASPECT_GAME);
    close_to(aspect_window_ratio(0, 0), ASPECT_GAME);
    close_to(aspect_window_ratio(100000, 1), ASPECT_GAME);
    setting = "16:9";
    aspect_update(5120, 1440);
    assert(!enabled);
    setting = "invalid";
    aspect_update(3440, 1440);
    assert(!enabled);
    puts("aspect: live setting, unsupported build, 21:9/32:9 resize and HUD rectangles passed");
}
