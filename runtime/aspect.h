// Ultrawide camera and centred HUD. The original 16:9 mode leaves the game's projections and draws unchanged.
#pragma once
#include <stdint.h>

#define ASPECT_GAME (16.0f / 9.0f)

// Only widen the view: taller windows keep the game's vertical field of view and its original aspect.
static inline float aspect_window_ratio(unsigned w, unsigned h) {
    float a = h ? (float)w / h : ASPECT_GAME;
    return a > ASPECT_GAME && a <= 8.0f ? a : ASPECT_GAME;
}

// Transform a viewport or scissor about the centre of its render target, keeping the HUD's original proportions.
static inline void aspect_ui_span(float centre, float scale, float *start, float *size) {
    *start = centre + (*start - centre) / scale;
    *size /= scale;
}

void aspect_init(void);                      // after rt_patches_init, before the game starts
int aspect_supported(void);                  // this executable has the checked camera patch sites
void aspect_update(unsigned w, unsigned h);   // present thread, the window's current drawable size
float aspect_ratio(void);                    // replay thread, 16:9 when disabled
