/**
 * @file TimeManager.h
 * @brief Manages NTP and RTC (DS3231) time synchronization.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <RTClib.h>

#include <ctime>

class TimeManager {
   public:
	/**
	 * @brief Initialize RTC and configure NTP synchronization.
	 */
	void begin();

	/** @brief Get current time from the system (NTP/RTC synced). */
	time_t getCurrentTime() const;

	/** @brief Returns true if the time has been set reasonably. */
	bool isTimeSynced() const;

   private:
	static RTC_DS3231 rtc;
	static bool rtcFound;

	/** @brief Callback invoked by SNTP when time is received from the network.
	 */
	static void timeSyncCallback(struct timeval *tv);
};
