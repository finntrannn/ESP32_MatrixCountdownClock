/**
 * @file NetworkManager.cpp
 * @brief Implementation of WiFi and NTP management.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "NetworkManager.h"
#include "Config.h"

void NetworkManager::begin(const AppState& appState) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(appState.getApSSID().c_str(), appState.getApPass().c_str());
    WiFi.begin(appState.getStaSSID().c_str(), appState.getStaPass().c_str());

    Serial.println("Web Server bắt đầu tại IP: 192.168.4.1");

    // Start Captive Portal DNS Server (redirect all domains to 192.168.4.1)
    dnsServer_.start(53, "*", WiFi.softAPIP());
}

bool NetworkManager::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::loop() {
    dnsServer_.processNextRequest();
}
