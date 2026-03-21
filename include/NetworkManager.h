/**
 * @file NetworkManager.h
 * @brief WiFi and NTP time synchronization management.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ctime>

class NetworkManager {
public:
    /**
     * @brief Start WiFi in AP+STA mode and configure NTP.
     *
     * Sets up a soft access point for configuration and simultaneously
     * connects to the configured WiFi network for internet/NTP access.
     */
    void begin();

    /** @brief Check if the station is connected to the WiFi network. */
    bool isConnected() const;

    /** @brief Get the current time from the system clock (NTP-synced). */
    time_t getCurrentTime() const;

    /** @brief Check if the time has been successfully synced via NTP. */
    bool isTimeSynced() const;
};
