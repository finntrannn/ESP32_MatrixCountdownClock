/**
 * @file WeatherScreen.h
 * @brief Renders the indoor/outdoor weather dashboard (DHT11 + Open-Meteo).
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include "AppState.h"
#include "DHTSensor.h"
#include "DisplayManager.h"
class WeatherScreen {
   public:
	/**
	 * @brief Resets the slide-in animation state (called when switching to this
	 * screen).
	 */
	void resetAnimations();

	/**
	 * @brief Render the sensor viewing screen.
	 * @param dt Delta time in seconds.
	 * @param display DisplayManager reference.
	 * @param dhtSensor DHTSensor reference.
	 * @param appState AppState reference.
	 */
	void draw(float dt, DisplayManager& display, const DHTSensor& dhtSensor,
			  const AppState& appState);

   private:
	float sensorBarSmooth_t_ = 0.0f;
	float sensorBarSmooth_h_ = 0.0f;
};
