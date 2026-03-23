#pragma once

#include "DisplayManager.h"
#include "AppState.h"
#include <U8g2_for_Adafruit_GFX.h>

class TextScreen {
public:
    TextScreen();
    void draw(float dt, DisplayManager& display, const AppState& appState);
    void resetAnimations();

private:
    float textX_;
    float scrollAccum_ = 0.0f;
    U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
    bool u8g2_initialized_ = false;
};
