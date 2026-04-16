/**
 * @file WeatherManager.cpp
 * @brief Implementation of the background weather fetching task.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "WeatherManager.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>

void WeatherManager::begin(AppState& state, NetworkManager& network) {
	state_	 = &state;
	network_ = &network;

	// Stack 8192: HTTPClient + ArduinoJson need significant stack space
	xTaskCreatePinnedToCore(
		WeatherManager::weatherTask,
		"Weather_Task",
		8192,
		this,
		1,
		&taskHandle_,
		0
	);
}

void WeatherManager::weatherTask(void* pvParameters) {
	WeatherManager* instance = static_cast<WeatherManager*>(pvParameters);
	unsigned long lastFetch = 0;

	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(5000));

		if (!instance->state_->isOpenMeteoEnabled()) {
			lastFetch = 0;
			continue;
		}

		if (!instance->network_->isConnected()) {
			continue;
		}

		bool timeToFetch = (lastFetch == 0) || (millis() - lastFetch > 900000);
		if (!instance->state_->isRealDataValid() && millis() - lastFetch > 30000) {
			timeToFetch = true;
		}

		if (timeToFetch) {
			Serial.println("[Weather] Starting fetch...");
			instance->fetchWeather();
			lastFetch = millis();
		}
	}
}

void WeatherManager::fetchWeather() {
	float lat = state_->getWeatherLat();
	float lon = state_->getWeatherLon();

	String url = "http://api.open-meteo.com/v1/forecast?latitude=";
	url += String(lat, 4);
	url += "&longitude=";
	url += String(lon, 4);
	url += "&current=temperature_2m,relative_humidity_2m";

	Serial.printf("[Weather] URL: %s\n", url.c_str());

	HTTPClient http;
	http.setTimeout(10000);

	if (!http.begin(url)) {
		Serial.println("[Weather] http.begin() failed");
		return;
	}

	int httpCode = http.GET();
	Serial.printf("[Weather] HTTP code: %d\n", httpCode);

	if (httpCode != HTTP_CODE_OK) {
		Serial.printf("[Weather] HTTP Error: %d\n", httpCode);
		http.end();
		state_->setRealWeatherData(0.0f, 0.0f, false);
		return;
	}

	String payload = http.getString();
	http.end();

	Serial.printf("[Weather] Payload (%d bytes): %.100s\n",
				  payload.length(), payload.c_str());

	JsonDocument doc;
	DeserializationError error = deserializeJson(doc, payload);

	if (error) {
		Serial.printf("[Weather] JSON error: %s\n", error.c_str());
		state_->setRealWeatherData(0.0f, 0.0f, false);
		return;
	}

	// ArduinoJson v7: use .is<T>() instead of deprecated containsKey()
	if (!doc["current"].is<JsonObject>()) {
		Serial.println("[Weather] Missing 'current' object");
		state_->setRealWeatherData(0.0f, 0.0f, false);
		return;
	}

	JsonObject current = doc["current"];
	if (!current["temperature_2m"].is<float>() &&
		!current["temperature_2m"].is<int>()) {
		Serial.println("[Weather] Missing temperature_2m");
		state_->setRealWeatherData(0.0f, 0.0f, false);
		return;
	}

	float temp = current["temperature_2m"].as<float>();
	float humi = current["relative_humidity_2m"].as<float>();

	state_->setRealWeatherData(temp, humi, true);
	Serial.printf("[Weather] OK: T=%.1f°C  H=%.0f%%\n", temp, humi);
}
