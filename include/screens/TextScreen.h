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
	int scrollPos_    = 64;	 // Current pixel position (integer only)
	int scrollAccum_  = 0;	 // Bresenham fractional accumulator
	U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
	
	String lastText_;
	bool lastScrollEnabled_ = false;
	int lastSize_ = -1;
	int cachedTextWidth_ = -1;

	GFXcanvas1* textCanvas_ = nullptr;
	int canvasWidth_ = 0;
	int canvasHeight_ = 0;
};
