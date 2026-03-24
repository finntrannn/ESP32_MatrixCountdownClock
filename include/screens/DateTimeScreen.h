/**
 * @file DateTimeScreen.h
 * @brief Renders the Date, Time, Temperature, and Humidity interface.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "DisplayManager.h"
#include "TimeManager.h"
#include "AppState.h"

class DateTimeScreen {
public:
    /**
     * @brief Render the DateTime screen.
     * @param dt Delta time for animations.
     * @param display DisplayManager reference.
     * @param timeManager TimeManager reference.
     * @param appState AppState reference.
     */
    void draw(float dt, DisplayManager& display, TimeManager& timeManager, const AppState& appState);
};
