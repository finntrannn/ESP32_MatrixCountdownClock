/**
 * @file WebConfigServer.cpp
 * @brief Implementation of the web-based configuration portal.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "WebConfigServer.h"

#include <ArduinoJson.h>
#include <Update.h>

#include "WebStrings.h"
#include "WebUI_Static.h"

static volatile bool shouldReboot_ = false;



void WebConfigServer::begin(AppState &state, DisplayManager &display,
							SplashScreen &splash, FireworksEffect &fireworks) {
	state_	   = &state;
	display_   = &display;
	splash_	   = &splash;
	fireworks_ = &fireworks;

	server_.on("/", HTTP_GET,
			   [this](AsyncWebServerRequest *request) { handleRoot(request); });
	server_.on("/api/config", HTTP_GET,
			   [this](AsyncWebServerRequest *request) { handleGetConfig(request); });
	server_.on("/save", HTTP_POST,
			   [this](AsyncWebServerRequest *request) { handleSave(request); });
	server_.on("/restart", HTTP_POST, [this](AsyncWebServerRequest *request) {
		handleRestart(request);
	});
	server_.on(
		"/showsplash", HTTP_POST,
		[this](AsyncWebServerRequest *request) { handleShowSplash(request); });
	server_.on("/test-fireworks", HTTP_POST,
			   [this](AsyncWebServerRequest *request) {
				   handleTestFireworks(request);
			   });

	// Spinner AJAX endpoints
	server_.on("/spin", HTTP_POST, [this](AsyncWebServerRequest *request) {
		state_->requestSpin();
		request->send(200, "text/plain", "OK");
	});
	server_.on("/spin-reset", HTTP_POST, [this](AsyncWebServerRequest *request) {
		state_->requestSpinReset();
		request->send(200, "text/plain", "OK");
	});

	server_.on(
		"/update", HTTP_POST,
		[](AsyncWebServerRequest *request) {
			shouldReboot_					 = !Update.hasError();
			AsyncWebServerResponse *response = request->beginResponse(
				200, "text/plain", shouldReboot_ ? "OK" : "FAIL");
			response->addHeader("Connection", "close");
			request->send(response);
		},
		[](AsyncWebServerRequest *request, String filename, size_t index,
		   uint8_t *data, size_t len, bool final) {
			if (!index) {
				Serial.printf("Update: %s\n", filename.c_str());
				if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
					Update.printError(Serial);
				}
			}
			if (!Update.hasError()) {
				if (Update.write(data, len) != len) {
					Update.printError(Serial);
				}
			}
			if (final) {
				if (Update.end(true)) {
					Serial.printf("Update Success: %uB\n", index + len);
				} else {
					Update.printError(Serial);
				}
			}
		});

	server_.onNotFound([](AsyncWebServerRequest *request) {
		request->redirect("http://192.168.4.1/");
	});

	server_.begin();
}

void WebConfigServer::loop() {
	if (shouldReboot_) {
		shouldReboot_ = false;
		delay(500);
		ESP.restart();
	}
}

void WebConfigServer::handleRoot(AsyncWebServerRequest *request) {
	request->send(200, "text/html", HTML_UI_STATIC);
}

void WebConfigServer::handleGetConfig(AsyncWebServerRequest *request) {
	JsonDocument doc;

	doc["staSSID"] = state_->getStaSSID();
	doc["staPass"] = state_->getStaPass();
	doc["apSSID"] = state_->getApSSID();
	doc["apPass"] = state_->getApPass();

	doc["ledState"] = state_->isLedEnabled();
	doc["splashState"] = state_->isSplashEnabled();
	doc["splashText"] = state_->getSplashText();
	doc["wifiIcon"] = state_->isWifiIconEnabled();
	doc["brightness"] = state_->getBrightness();
	doc["screenMode"] = state_->getScreenMode();

	doc["showSecCD"] = state_->isShowSecondsCountdownEnabled();
	doc["showMessage"] = state_->isShowMessageEnabled();
	doc["rainbowState"] = state_->isRainbowEnabled();
	doc["targetEpoch"] = state_->getTargetEpoch();
	doc["customText"] = state_->getCustomText();
	doc["textSpeed"] = state_->getTextSpeed();
	doc["rainbowSpeed"] = state_->getRainbowSpeed();
	doc["cdDaysColor"] = state_->getCdDaysColor();
	doc["cdTimeColor"] = state_->getCdTimeColor();

	doc["sensorBg"] = state_->isSensorBgEnabled();
	doc["openMeteoEn"] = state_->isOpenMeteoEnabled();
	doc["weaLat"] = state_->getWeatherLat();
	doc["weaLon"] = state_->getWeatherLon();

	doc["showYear"] = state_->isShowYearEnabled();
	doc["shortYear"] = state_->isShortYearEnabled();
	doc["showSecDT"] = state_->isShowSecondsDateTimeEnabled();
	doc["dateSep"] = state_->getDateSeparator();

	doc["txtScroll"] = state_->isTextPanelScrollEnabled();
	doc["txtContent"] = state_->getTextPanelContent();
	doc["txtSpeed"] = state_->getTextPanelSpeed();
	doc["txtSize"] = state_->getTextPanelSize();
	doc["txtColor"] = state_->getTextPanelColor();

	doc["autoCycle"] = state_->isAutoCycleEnabled();
	doc["cycleSeq"] = state_->getCycleSequence();
	doc["timer0"] = state_->getTimer0_Countdown();
	doc["timer1"] = state_->getTimer1_Sensor();
	doc["timer2"] = state_->getTimer2_DateTime();
	doc["timer3"] = state_->getTimer3_Text();
	doc["timer4"] = state_->getTimer4_Spinner();
	doc["fwScheds"] = state_->getFwSchedules();

	doc["spinMin"] = state_->getSpinRangeMin();
	doc["spinMax"] = state_->getSpinRangeMax();
	doc["spinDur"] = state_->getSpinDuration();

	String responseStr;
	serializeJson(doc, responseStr);
	request->send(200, "application/json", responseStr);
}

void WebConfigServer::handleSave(AsyncWebServerRequest *request) {
	auto hasArg = [&](const String &n) {
		return request->hasParam(n, true) || request->hasParam(n, false);
	};
	auto arg = [&](const String &n) {
		if (request->hasParam(n, true))
			return request->getParam(n, true)->value();
		if (request->hasParam(n, false))
			return request->getParam(n, false)->value();
		return String("");
	};

	state_->lock();

	if (hasArg("datetime")) {
		String dt = arg("datetime");
		int yr, mo, da, hr, mn;
		if (sscanf(dt.c_str(), "%d-%d-%dT%d:%d", &yr, &mo, &da, &hr, &mn) ==
			5) {
			struct tm tm_target = {0};
			tm_target.tm_year	= yr - 1900;
			tm_target.tm_mon	= mo - 1;
			tm_target.tm_mday	= da;
			tm_target.tm_hour	= hr;
			tm_target.tm_min	= mn;
			tm_target.tm_sec	= 0;
			tm_target.tm_isdst	= -1;
			time_t newEpoch		= mktime(&tm_target);
			if (newEpoch != -1) state_->setTargetEpoch(newEpoch);
		}
	}

	state_->setLedEnabled(hasArg("ledState") && arg("ledState") == "1");
	state_->setSplashEnabled(hasArg("splashState") &&
							 arg("splashState") == "1");
	splash_->setEnabled(state_->isSplashEnabled());
	if (hasArg("splashText")) state_->setSplashText(arg("splashText"));

	if (hasArg("brightness")) {
		state_->setBrightness(arg("brightness").toInt());
		display_->setBrightness(state_->getBrightness());
	}

	if (hasArg("screenMode")) state_->setScreenMode(arg("screenMode").toInt());

	if (hasArg("apSSID")) state_->setApSSID(arg("apSSID"));
	if (hasArg("apPass")) state_->setApPass(arg("apPass"));
	if (hasArg("staSSID")) state_->setStaSSID(arg("staSSID"));
	if (hasArg("staPass")) state_->setStaPass(arg("staPass"));

	state_->setShowYearEnabled(hasArg("showYear") && arg("showYear") == "1");
	state_->setShortYearEnabled(hasArg("shortYear") && arg("shortYear") == "1");
	if (hasArg("dateSep")) state_->setDateSeparator(arg("dateSep").toInt());
	state_->setShowSecondsDateTimeEnabled(hasArg("showSecDT") &&
										  arg("showSecDT") == "1");

	if (hasArg("customText")) state_->setCustomText(arg("customText"));

	state_->setShowSecondsCountdownEnabled(hasArg("showSecCD") &&
										   arg("showSecCD") == "1");
	state_->setShowMessageEnabled(hasArg("showMessage") &&
								  arg("showMessage") == "1");
	state_->setRainbowEnabled(hasArg("rainbowState") &&
							  arg("rainbowState") == "1");

	if (hasArg("cdDaysClr")) state_->setCdDaysColor(arg("cdDaysClr"));
	if (hasArg("cdTimeClr")) state_->setCdTimeColor(arg("cdTimeClr"));

	if (hasArg("textSpeed")) state_->setTextSpeed(arg("textSpeed").toInt());
	if (hasArg("rainbowSpeed"))
		state_->setRainbowSpeed(arg("rainbowSpeed").toInt());

	state_->setSensorBgEnabled(hasArg("sensorBg") && arg("sensorBg") == "1");
	state_->setOpenMeteoEnabled(hasArg("openMeteoEn") && arg("openMeteoEn") == "1");
	if (hasArg("weaLat")) state_->setWeatherLat(arg("weaLat").toFloat());
	if (hasArg("weaLon")) state_->setWeatherLon(arg("weaLon").toFloat());

	state_->setWifiIconEnabled(hasArg("wifiIcon") && arg("wifiIcon") == "1");

	if (hasArg("txtContent")) state_->setTextPanelContent(arg("txtContent"));
	if (hasArg("txtSpeed")) state_->setTextPanelSpeed(arg("txtSpeed").toInt());
	if (hasArg("txtSize")) state_->setTextPanelSize(arg("txtSize").toInt());
	if (hasArg("txtColor")) state_->setTextPanelColor(arg("txtColor"));
	state_->setTextPanelScrollEnabled(hasArg("txtScroll") &&
									  arg("txtScroll") == "1");

	state_->setAutoCycleEnabled(hasArg("autoCycle") && arg("autoCycle") == "1");
	if (hasArg("timer0")) state_->setTimer0_Countdown(arg("timer0").toInt());
	if (hasArg("timer1")) state_->setTimer1_Sensor(arg("timer1").toInt());
	if (hasArg("timer2")) state_->setTimer2_DateTime(arg("timer2").toInt());
	if (hasArg("timer3")) state_->setTimer3_Text(arg("timer3").toInt());
	if (hasArg("cycleSeq")) state_->setCycleSequence(arg("cycleSeq"));
	if (hasArg("fwScheds")) state_->setFwSchedules(arg("fwScheds"));

	// Spinner settings
	if (hasArg("spinMin")) state_->setSpinRangeMin(arg("spinMin").toInt());
	if (hasArg("spinMax")) state_->setSpinRangeMax(arg("spinMax").toInt());
	if (hasArg("spinDur")) state_->setSpinDuration(arg("spinDur").toInt());
	if (hasArg("timer4")) state_->setTimer4_Spinner(arg("timer4").toInt());

	state_->unlock();

	request->send(200, "application/json", "{\"status\":\"ok\"}");
}

void WebConfigServer::handleRestart(AsyncWebServerRequest *request) {
	using namespace WebUI;
	request->send(200, "text/html",
				  "<html><head><meta charset='utf-8'><meta name='viewport' "
				  "content='width=device-width, initial-scale=1'>"
				  "</head><body "
				  "style='background:#f0f2f5;color:#1c1e21;text-align:center;"
				  "font-family:Arial;padding-top:50px;'>"
				  "<h2>" +
					  String(kRestartMsg) +
					  "</h2>"
					  "<p>" +
					  String(kRestartNote) +
					  "</p>"
					  "</body></html>");
	shouldReboot_ = true;
}

void WebConfigServer::handleShowSplash(AsyncWebServerRequest *request) {
	using namespace WebUI;
	request->send(200, "text/html",
				  "<html><head><meta charset='utf-8'><meta name='viewport' "
				  "content='width=device-width, initial-scale=1'>"
				  "<meta http-equiv='refresh' content='11;url=/'></head>"
				  "<body "
				  "style='background:#f0f2f5;color:#1c1e21;text-align:center;"
				  "font-family:Arial;padding-top:50px;'>"
				  "<h2>" +
					  String(kSplashMsg) +
					  "</h2>"
					  "<p>" +
					  String(kSplashNote) +
					  "</p>"
					  "</body></html>");
	state_->requestSplashPlay();
}

void WebConfigServer::handleTestFireworks(AsyncWebServerRequest *request) {
	fireworks_->trigger(5);
	request->send(200, "text/plain", "OK");
}
