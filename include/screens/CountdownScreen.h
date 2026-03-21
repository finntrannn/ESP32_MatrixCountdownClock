/**
 * @file CountdownScreen.h
 * @brief Renders the THPT QG Countdown interface.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "DisplayManager.h"
#include "NetworkManager.h"
#include "AppState.h"

class CountdownScreen {
public:
    /**
     * @brief Render the countdown screen.
     * @param dt Delta time in seconds.
     * @param display DisplayManager reference.
     * @param network NetworkManager reference (for time and status).
     * @param appState AppState reference (for target epoch, text, etc.).
     */
    void draw(float dt, DisplayManager& display, NetworkManager& network, const AppState& appState);

private:
    float textX_ = 64.0f;
    float borderFloatOffset_ = 0.0f;
};
