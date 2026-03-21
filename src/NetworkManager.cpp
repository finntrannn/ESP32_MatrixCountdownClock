/**
 * @file NetworkManager.cpp
 * @brief Implementation of WiFi and NTP management.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "NetworkManager.h"
#include "Config.h"

void NetworkManager::begin() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(WiFiConfig::kAPName, WiFiConfig::kAPPassword);
    WiFi.begin(WiFiConfig::kSSID, WiFiConfig::kPassword);

    configTime(NtpConfig::kGmtOffsetSec, NtpConfig::kDaylightOffset,
               NtpConfig::kServer1, NtpConfig::kServer2);

    Serial.println("Web Server bắt đầu tại IP: 192.168.4.1");
}

bool NetworkManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

time_t NetworkManager::getCurrentTime() const {
    return time(nullptr);
}

bool NetworkManager::isTimeSynced() const {
    return time(nullptr) > 1000000;
}
