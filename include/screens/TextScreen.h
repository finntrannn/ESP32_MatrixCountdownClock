/**
 * @file TextScreen.h
 * @brief Free-form text display screen with Vietnamese font support.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <U8g2_for_Adafruit_GFX.h>

#include "AppState.h"
#include "DisplayManager.h"

class TextScreen {
   public:
	TextScreen();
	void draw(float dt, DisplayManager& display, const AppState& appState);
	void resetAnimations();

   private:
	float textX_;
	U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
	bool u8g2_initialized_ = false;
};
