/**
 * @file Config.h
 * @brief Compile-time constants, pin definitions, and default values.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <cstdint>

// ─── LED Matrix Panel ────────────────────────────────────────────────
namespace Panel {
    constexpr int kResX   = 64;
    constexpr int kResY   = 32;
    constexpr int kChain  = 1;
}

// ─── DHT11 Sensor ────────────────────────────────────────────────────
namespace DHTConfig {
    constexpr uint8_t kPin = 32;   // GPIO pin connected to DHT11 data
}

// ─── WiFi Credentials ───────────────────────────────────────────────
namespace WiFiConfig {
    constexpr const char* kSSID       = "Belvedere";
    constexpr const char* kPassword   = "123789456@";
    constexpr const char* kAPName     = "THPT_QG_Config";
    constexpr const char* kAPPassword = "12345678";
}

// ─── Default Settings ───────────────────────────────────────────────
namespace Defaults {
    constexpr uint32_t kTargetEpoch   = 1782320400;  // 25/06/2026 00:00 GMT+7
    constexpr bool     kLedEnabled    = true;
    constexpr bool     kSplashEnabled = true;
    constexpr bool     kRainbowEnabled = true;
    constexpr int      kTextSpeed     = 4;   // 1 (slow) – 10 (fast)
    constexpr int      kRainbowSpeed  = 5;   // 1 (slow) – 10 (fast)
    constexpr bool     kShowSeconds   = true; // Show seconds in countdown
    constexpr int      kBrightness    = 60;  // 1 (dim) – 255 (max)
    constexpr const char* kCustomText = "Co len!";
    constexpr int      kScreenMode      = 0;     // 0 = Countdown, 1 = Sensor
    constexpr bool     kSensorBgEnabled = true; // Background gradient on sensor screen
}

// ─── NVS Preference Keys ────────────────────────────────────────────
namespace NvsKeys {
    constexpr const char* kNamespace   = "clock_app";
    constexpr const char* kTargetEpoch = "targetEpoch";
    constexpr const char* kLedState    = "ledState";
    constexpr const char* kSplashEn    = "splashEn";
    constexpr const char* kCustomText  = "customText";
    constexpr const char* kRainbow     = "rainbow";
    constexpr const char* kTextSpeed   = "textSpd";
    constexpr const char* kRainbowSpeed = "rbSpd";
    constexpr const char* kShowSecs    = "showSecs";
    constexpr const char* kBrightness  = "brightness";
    constexpr const char* kScreenMode    = "screenMode";
    constexpr const char* kSensorBg      = "sensBg";
}

// ─── NTP Configuration ──────────────────────────────────────────────
namespace NtpConfig {
    constexpr long   kGmtOffsetSec   = 7 * 3600;  // GMT+7
    constexpr int    kDaylightOffset = 0;
    constexpr const char* kServer1   = "pool.ntp.org";
    constexpr const char* kServer2   = "time.nist.gov";
}
