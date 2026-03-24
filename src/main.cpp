/**
 * @file main.cpp
 * @brief Entry point for the THPT QG Countdown Clock.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include <Arduino.h>

#include "AppState.h"
#include "Config.h"
#include "DHTSensor.h"
#include "DisplayManager.h"
#include "FireworksEffect.h"
#include "NetworkManager.h"
#include "SplashScreen.h"
#include "TimeManager.h"
#include "WebConfigServer.h"
#include "screens/CountdownScreen.h"
#include "screens/DateTimeScreen.h"
#include "screens/SensorScreen.h"
#include "screens/TextScreen.h"

// ─── Module Instances ────────────────────────────────────────────────
static AppState appState;
static DisplayManager display;
static NetworkManager network;
static TimeManager timeManager;
static WebConfigServer webConfig;
static SplashScreen splash;
static DHTSensor dhtSensor;

// ─── Screen Instances ────────────────────────────────────────────────
static CountdownScreen countdownScreen;
static SensorScreen sensorScreen;
static DateTimeScreen dateTimeScreen;
static TextScreen textScreen;
static FireworksEffect fireworks;

// Loop Animation State
static unsigned long lastAutoCycleMs = 0;

void displayTask(void *pvParameters) {
	unsigned long lastUpdateUs = micros();
	int prevScreenMode		   = -1;
	int prevFireworkMinute	   = -1;

	for (;;) {
		unsigned long nowUs = micros();
		float dt			= (nowUs - lastUpdateUs) / 1000000.0f;
		lastUpdateUs		= nowUs;

		if (dt > 0.5f) dt = 0.0f;

		if (appState.consumeSplashPlayRequest()) {
			splash.play(display, appState.getSplashText(), true);
			lastUpdateUs = micros();
			continue;
		}

		if (!appState.isLedEnabled()) {
			display.clear();
			display.flip();
			vTaskDelay(pdMS_TO_TICKS(100));
			continue;
		}

		display.clear();

		appState.lock();
		int currentMode = appState.getScreenMode();
		if (currentMode != prevScreenMode) {
			if (currentMode == 1)
				sensorScreen.resetAnimations();
			else if (currentMode == 3)
				textScreen.resetAnimations();
			prevScreenMode = currentMode;
		}

		if (currentMode == 1)
			sensorScreen.draw(dt, display, dhtSensor, appState);
		else if (currentMode == 2)
			dateTimeScreen.draw(dt, display, timeManager, appState);
		else if (currentMode == 3)
			textScreen.draw(dt, display, appState);
		else
			countdownScreen.draw(dt, display, timeManager, appState);

		time_t fwNow = timeManager.getCurrentTime();
		if (timeManager.isTimeSynced()) {
			struct tm *fwTime = localtime(&fwNow);
			int curMin		  = fwTime->tm_hour * 60 + fwTime->tm_min;
			bool isRecessTime = false;
			int fwDur		  = 0;

			const AppState::FwSchedule *scheds =
				appState.getParsedFwSchedules();
			for (int i = 0; i < 10; i++) {
				if (scheds[i].hour == -1) break;
				if (curMin == scheds[i].hour * 60 + scheds[i].minute) {
					isRecessTime = true;
					fwDur		 = scheds[i].duration;
					break;
				}
			}

			if (isRecessTime && fwTime->tm_sec == 0 &&
				prevFireworkMinute != curMin) {
				fireworks.trigger(fwDur > 0 ? fwDur : 5);
				prevFireworkMinute = curMin;
			}
			if (!isRecessTime) prevFireworkMinute = -1;
		}
		appState.unlock();

		// WiFi status indicator (all screens)
		if (appState.isWifiIconEnabled()) {
			auto *panel		 = display.getPanel();
			uint16_t wifiClr = network.isConnected()
								   ? panel->color565(0, 255, 0)
								   : panel->color565(255, 128, 0);
			panel->drawPixel(62, 30, wifiClr);
		}

		fireworks.update(dt, display);

		display.flip();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void setup() {
	Serial.begin(115200);
	Serial.println("Khoi dong THPT QG Counter...");

	// 1. Load user settings from NVS
	appState.load();

	// 2. Initialize LED matrix display
	display.begin(appState.getBrightness());

	// 3. Start WiFi (AP+STA)
	network.begin(appState);

	// 4. Khởi tạo và thiết lập đồng bộ thời gian (RTC + NTP)
	timeManager.begin();

	// 5. Initialize DHT11 sensor
	dhtSensor.begin(DHTConfig::kPin);

	splash.setEnabled(appState.isSplashEnabled());
	webConfig.begin(appState, display, splash, fireworks);

	splash.play(display, appState.getSplashText());

	xTaskCreatePinnedToCore(displayTask, "Display_Task", 8192, nullptr, 2,
							nullptr, 1);
}

void loop() {
	unsigned long nowMs = millis();

	// Process Captive Portal DNS + OTA reboot check
	network.loop();
	webConfig.loop();

	// Auto-Cycle Screen Logic
	if (appState.isAutoCycleEnabled()) {
		int timers[4] = {
			appState.getTimer0_Countdown(), appState.getTimer1_Sensor(),
			appState.getTimer2_DateTime(), appState.getTimer3_Text()};
		int currentMode		= appState.getScreenMode();
		int currentDuration = timers[currentMode];

		if (currentDuration > 0) {
			if (nowMs - lastAutoCycleMs >= currentDuration * 1000UL) {
				int seqLen	   = 0;
				const int *seq = appState.getParsedCycleSequence(seqLen);
				int nextMode   = currentMode;

				if (seqLen > 0) {
					int currIdx = -1;
					for (int i = 0; i < seqLen; i++) {
						if (seq[i] == currentMode) {
							currIdx = i;
							break;
						}
					}
					if (currIdx == -1) {
						for (int i = 0; i < seqLen; i++) {
							int checkMode = seq[i];
							if (checkMode >= 0 && checkMode < 4 &&
								timers[checkMode] > 0) {
								nextMode = checkMode;
								break;
							}
						}
					} else {
						for (int i = 1; i <= seqLen; i++) {
							int checkIdx  = (currIdx + i) % seqLen;
							int checkMode = seq[checkIdx];
							if (checkMode >= 0 && checkMode < 4 &&
								timers[checkMode] > 0) {
								nextMode = checkMode;
								break;
							}
						}
					}
				} else {
					for (int i = 1; i <= 4; i++) {
						int checkMode = (currentMode + i) % 4;
						if (timers[checkMode] > 0) {
							nextMode = checkMode;
							break;
						}
					}
				}

				if (nextMode != currentMode) {
					appState.setScreenModeVolatile(nextMode);
				}
				lastAutoCycleMs = nowMs;
			}
		} else {
			lastAutoCycleMs = nowMs;
		}
	} else {
		lastAutoCycleMs = nowMs;
	}

	vTaskDelay(pdMS_TO_TICKS(100));
}