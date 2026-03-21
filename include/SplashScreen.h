/**
 * @file SplashScreen.h
 * @brief Animated splash screen (12A6 animation) for startup.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "DisplayManager.h"

// Forward declaration to avoid pulling in WebServer.h
class WebServer;

class SplashScreen {
public:
    /**
     * @brief Play the full splash animation sequence.
     * @param display  Reference to the display manager (dependency injection).
     * @param server   Optional web server pointer — if non-null, handleClient()
     *                 is called during animation to keep the web portal responsive.
     * @param force    If true, plays even when splash is disabled in settings.
     */
    void play(DisplayManager& display, WebServer* server, bool force = false);

    /** @brief Set whether the splash screen is enabled. */
    void setEnabled(bool enabled) { enabled_ = enabled; }

    /** @brief Check if the splash screen is enabled. */
    bool isEnabled() const { return enabled_; }

private:
    bool enabled_ = true;

    /** @brief Internal star data for the star field effect. */
    struct Star {
        int x, y, speed;
    };
};
