/**
 * @file WeatherManager.h
 * @brief Background task to fetch Open-Meteo weather data.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "AppState.h"
#include "NetworkManager.h"

class WeatherManager {
   public:
	/**
	 * @brief Initialize and start the background fetching task.
	 * @param state Reference to application state.
	 * @param network Reference to network manager.
	 */
	void begin(AppState& state, NetworkManager& network);

   private:
	AppState* state_	  = nullptr;
	NetworkManager* network_ = nullptr;
	TaskHandle_t taskHandle_ = nullptr;

	static void weatherTask(void* pvParameters);
	void fetchWeather();
};
