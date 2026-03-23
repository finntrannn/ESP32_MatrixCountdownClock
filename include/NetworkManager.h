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
#include <DNSServer.h>
#include "AppState.h"

class NetworkManager {
public:
    /**
     * @brief Start WiFi in AP+STA mode using credentials from AppState.
     */
    void begin(const AppState& appState);

    /** @brief Check if the station is connected to the WiFi network. */
    bool isConnected() const;

    /** @brief Loop to handle DNS requests for Captive Portal */
    void loop();

private:
    DNSServer dnsServer_;
};
