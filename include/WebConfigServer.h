/**
 * @file WebConfigServer.h
 * @brief Web-based configuration portal for the countdown clock.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <ESPAsyncWebServer.h>

#include "AppState.h"
#include "DisplayManager.h"
#include "FireworksEffect.h"
#include "SplashScreen.h"

class WebConfigServer {
   public:
	/**
	 * @brief Initialize the web server and register all routes.
	 * @param state    Reference to the application state (for read/write
	 * settings).
	 * @param display  Reference to display manager (for brightness changes).
	 * @param splash   Reference to splash screen (for test-play trigger).
	 * @param fireworks Reference to fireworks effect (for test trigger).
	 */
	void begin(AppState& state, DisplayManager& display, SplashScreen& splash,
			   FireworksEffect& fireworks);
	void loop();

	AsyncWebServer* getServer() {
		return &server_;
	}

   private:
	AsyncWebServer server_{80};

	AppState* state_			= nullptr;
	DisplayManager* display_	= nullptr;
	SplashScreen* splash_		= nullptr;
	FireworksEffect* fireworks_ = nullptr;

	void handleRoot(AsyncWebServerRequest* request);
	void handleSave(AsyncWebServerRequest* request);
	void handleRestart(AsyncWebServerRequest* request);
	void handleShowSplash(AsyncWebServerRequest* request);
	void handleTestFireworks(AsyncWebServerRequest* request);
};
