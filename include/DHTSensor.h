/**
 * @file DHTSensor.h
 * @brief DHT11 temperature & humidity sensor management.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <DHTesp.h>

class DHTSensor {
   public:
	/**
	 * @brief Initialize the DHT11 sensor and start the background read task.
	 * @param pin GPIO pin connected to the DHT11 data line.
	 */
	void begin(uint8_t pin);

	/** @brief Last stable temperature reading in °C (deadband-filtered). */
	float getTemperature() const {
		return stableTemp_;
	}

	/** @brief Last stable humidity reading in % (deadband-filtered). */
	float getHumidity() const {
		return stableHumi_;
	}

	/** @brief True if at least one successful read has occurred. */
	bool isReady() const {
		return ready_;
	}

   private:
	DHTesp dht_;

	// Stable (deadband-filtered) values exposed to the renderer
	volatile float stableTemp_ = 0.0f;
	volatile float stableHumi_ = 0.0f;
	volatile bool ready_	   = false;

	// Deadband thresholds — ignore changes smaller than these
	static constexpr float kTempDeadband = 0.5f;  // °C
	static constexpr float kHumiDeadband = 2.0f;  // %

	// Read interval (DHT11 is slow and noisy at fast rates)
	static constexpr unsigned long kReadIntervalMs = 10000;	 // 10 seconds

	/** @brief FreeRTOS task function — runs sensor reads in the background. */
	static void readTask(void* param);
};
