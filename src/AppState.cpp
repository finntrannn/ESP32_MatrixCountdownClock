/**
 * @file AppState.cpp
 * @brief Implementation of NVS-backed application state.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "AppState.h"
#include "Config.h"

void AppState::load() {
    preferences_.begin(NvsKeys::kNamespace, false);

    targetEpoch_   = preferences_.getUInt(NvsKeys::kTargetEpoch, Defaults::kTargetEpoch);
    ledEnabled_    = preferences_.getBool(NvsKeys::kLedState,    Defaults::kLedEnabled);
    splashEnabled_ = preferences_.getBool(NvsKeys::kSplashEn,    Defaults::kSplashEnabled);
    customText_    = preferences_.getString(NvsKeys::kCustomText, Defaults::kCustomText);
    rainbowEnabled_ = preferences_.getBool(NvsKeys::kRainbow,    Defaults::kRainbowEnabled);
    showSeconds_   = preferences_.getBool(NvsKeys::kShowSecs,    Defaults::kShowSeconds);
    textSpeed_     = preferences_.getInt(NvsKeys::kTextSpeed,    Defaults::kTextSpeed);
    rainbowSpeed_  = preferences_.getInt(NvsKeys::kRainbowSpeed, Defaults::kRainbowSpeed);
    brightnessVal_ = preferences_.getInt(NvsKeys::kBrightness,   Defaults::kBrightness);
    screenMode_      = preferences_.getInt(NvsKeys::kScreenMode,    Defaults::kScreenMode);
    sensorBgEnabled_ = preferences_.getBool(NvsKeys::kSensorBg,      Defaults::kSensorBgEnabled);
}

void AppState::setTargetEpoch(time_t epoch) {
    targetEpoch_ = epoch;
    preferences_.putUInt(NvsKeys::kTargetEpoch, static_cast<uint32_t>(epoch));
}

void AppState::setLedEnabled(bool enabled) {
    ledEnabled_ = enabled;
    preferences_.putBool(NvsKeys::kLedState, enabled);
}

void AppState::setSplashEnabled(bool enabled) {
    splashEnabled_ = enabled;
    preferences_.putBool(NvsKeys::kSplashEn, enabled);
}

void AppState::setRainbowEnabled(bool enabled) {
    rainbowEnabled_ = enabled;
    preferences_.putBool(NvsKeys::kRainbow, enabled);
}

void AppState::setShowSeconds(bool enabled) {
    showSeconds_ = enabled;
    preferences_.putBool(NvsKeys::kShowSecs, enabled);
}

void AppState::setTextSpeed(int speed) {
    textSpeed_ = speed;
    preferences_.putInt(NvsKeys::kTextSpeed, speed);
}

void AppState::setRainbowSpeed(int speed) {
    rainbowSpeed_ = speed;
    preferences_.putInt(NvsKeys::kRainbowSpeed, speed);
}

void AppState::setBrightness(int brightness) {
    if (brightness < 1) brightness = 1;
    if (brightness > 255) brightness = 255;
    brightnessVal_ = brightness;
    preferences_.putInt(NvsKeys::kBrightness, brightness);
}

void AppState::setScreenMode(int mode) {
    screenMode_ = mode;
    preferences_.putInt(NvsKeys::kScreenMode, mode);
}

void AppState::setSensorBgEnabled(bool enabled) {
    sensorBgEnabled_ = enabled;
    preferences_.putBool(NvsKeys::kSensorBg, enabled);
}

void AppState::setCustomText(const String& text) {
    customText_ = text;
    preferences_.putString(NvsKeys::kCustomText, text);
}
