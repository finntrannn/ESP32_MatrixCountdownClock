/**
 * @file RandomSpinnerScreen.h
 * @brief Random number spinner/lottery screen with animation effects.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "AppState.h"
#include "DisplayManager.h"

class RandomSpinnerScreen {
   public:
	void draw(float dt, DisplayManager& display, AppState& appState);
	void resetAnimations();

   private:
	enum State { IDLE, SPINNING, RESULT };
	State state_ = IDLE;

	float spinTimer_	   = 0.0f;
	float spinDuration_	   = 5.0f;
	float flickerTimer_	   = 0.0f;
	float resultFlashTimer_ = 0.0f;
	int currentDisplayNum_ = 0;
	int finalResult_	   = 0;

	// Excluded numbers tracking (runtime copy)
	int excludedList_[200];
	int excludedCount_ = 0;

	void parseExcluded(const String& csv);
	bool isExcluded(int num);
	int pickRandom(int minVal, int maxVal);
	void drawNumber(DisplayManager& display, int num, uint16_t color);
	void drawCenteredText(DisplayManager& display, const char* text, int y,
						  uint16_t color);
};
