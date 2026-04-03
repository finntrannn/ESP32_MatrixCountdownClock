/**
 * @file CountdownScreen.h
 * @brief Renders the THPT QG Countdown interface.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "AppState.h"
#include "DisplayManager.h"
#include "TimeManager.h"

class CountdownScreen {
   public:
	/**
	 * @brief Render the countdown screen.
	 * @param dt Delta time in seconds.
	 * @param display DisplayManager reference.
	 * @param timeManager TimeManager reference (for time).
	 * @param appState AppState reference (for target epoch, text, etc.).
	 */
	void draw(float dt, DisplayManager& display, TimeManager& timeManager,
			  const AppState& appState);

   private:
	float textX_			 = 64.0f;
	float borderFloatOffset_ = 0.0f;
};
