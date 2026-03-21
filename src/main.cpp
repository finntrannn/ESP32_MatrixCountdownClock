/**
 * @file main.cpp
 * @brief Entry point for the THPT QG Countdown Clock.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include <Arduino.h>
#include "Config.h"
#include "AppState.h"
#include "DisplayManager.h"
#include "NetworkManager.h"
#include "WebConfigServer.h"
#include "SplashScreen.h"
#include "DHTSensor.h"
#include "screens/CountdownScreen.h"
#include "screens/SensorScreen.h"

// ─── Module Instances ────────────────────────────────────────────────
static AppState appState;
static DisplayManager display;
static NetworkManager network;
static WebConfigServer webConfig;
static SplashScreen splash;
static DHTSensor dhtSensor;

// ─── Screen Instances ────────────────────────────────────────────────
static CountdownScreen countdownScreen;
static SensorScreen sensorScreen;

// ─── Loop Animation State ────────────────────────────────────────────
static unsigned long lastUpdate = 0;
static int prevScreenMode = -1;

void setup()
{
	Serial.begin(115200);
	Serial.println("Khoi dong THPT QG Counter...");

	// 1. Load user settings from NVS
	appState.load();

	// 2. Initialize LED matrix display
	display.begin(appState.getBrightness());

	// 3. Start WiFi (AP+STA) and configure NTP — runs in background
	network.begin();

	// 4. Initialize DHT11 sensor
	dhtSensor.begin(DHTConfig::kPin);

	// 5. Start web configuration server (inject dependencies)
	splash.setEnabled(appState.isSplashEnabled());
	webConfig.begin(appState, display, splash);

	// 6. Play splash animation while WiFi connects in background
	splash.play(display, webConfig.getServer());

	lastUpdate = millis();
}

void loop()
{
	unsigned long nowMs = millis();
	float dt = (nowMs - lastUpdate) / 1000.0f;
	lastUpdate = nowMs;

	// Clamp dt to avoid animation jumps after WiFi stalls
	if (dt > 0.5f)
		dt = 0.0f;

	// Process incoming web requests
	webConfig.handleClient();

	// If LED is disabled, blank the display and yield
	if (!appState.isLedEnabled())
	{
		display.clear();
		display.flip();
		vTaskDelay(pdMS_TO_TICKS(100));
		return;
	}

	// ── Detect screen mode change → reset bar animations ────────
	int currentMode = appState.getScreenMode();
	if (currentMode != prevScreenMode)
	{
		if (currentMode == 1)
		{
			sensorScreen.resetAnimations();
		}
		prevScreenMode = currentMode;
	}

	display.clear();

	// ── Screen Mode Switch ───────────────────────────────────────
	if (currentMode == 1)
	{
		sensorScreen.draw(dt, display, dhtSensor, appState);
	}
	else
	{
		countdownScreen.draw(dt, display, network, appState);
	}

	// ── Flip Buffer ──────────────────────────────────────────────
	display.flip();

	// Yield to FreeRTOS
	vTaskDelay(pdMS_TO_TICKS(10));
}