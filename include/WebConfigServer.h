/**
 * @file WebConfigServer.h
 * @brief Web-based configuration portal for the countdown clock.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <WebServer.h>
#include "AppState.h"
#include "DisplayManager.h"
#include "SplashScreen.h"

class WebConfigServer {
public:
    /**
     * @brief Initialize the web server and register all routes.
     * @param state    Reference to the application state (for read/write settings).
     * @param display  Reference to display manager (for brightness changes).
     * @param splash   Reference to splash screen (for test-play trigger).
     */
    void begin(AppState& state, DisplayManager& display, SplashScreen& splash);

    /** @brief Process incoming HTTP client requests. Call in loop(). */
    void handleClient();

    /** @brief Get raw WebServer pointer (used by SplashScreen during animation). */
    WebServer* getServer() { return &server_; }

private:
    WebServer server_{80};

    // Injected dependencies (set during begin)
    AppState*       state_   = nullptr;
    DisplayManager* display_ = nullptr;
    SplashScreen*   splash_  = nullptr;

    /** @brief Serve the main configuration HTML page. */
    void handleRoot();

    /** @brief Process the form submission and persist settings. */
    void handleSave();

    /** @brief Restart the ESP32. */
    void handleRestart();

    /** @brief Trigger splash screen playback from the web UI. */
    void handleShowSplash();
};
