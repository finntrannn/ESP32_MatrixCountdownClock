/**
 * @file AppState.h
 * @brief Centralized application state with NVS persistence.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <ctime>

class AppState {
public:
    /**
     * @brief Load all settings from NVS flash. Call once in setup().
     */
    void load();

    // ─── Getters ─────────────────────────────────────────────────
    time_t   getTargetEpoch()   const { return targetEpoch_; }
    bool     isLedEnabled()     const { return ledEnabled_; }
    bool     isSplashEnabled()  const { return splashEnabled_; }
    bool     isRainbowEnabled() const { return rainbowEnabled_; }
    bool     isShowSecondsEnabled() const { return showSeconds_; }
    int      getTextSpeed()     const { return textSpeed_; }
    int      getRainbowSpeed()  const { return rainbowSpeed_; }
    int      getBrightness()    const { return brightnessVal_; }
    int      getScreenMode()       const { return screenMode_; }
    bool     isSensorBgEnabled()   const { return sensorBgEnabled_; }
    const String& getCustomText()  const { return customText_; }

    // ─── Setters (auto-persist to NVS) ───────────────────────────
    void setTargetEpoch(time_t epoch);
    void setLedEnabled(bool enabled);
    void setSplashEnabled(bool enabled);
    void setRainbowEnabled(bool enabled);
    void setShowSeconds(bool enabled);
    void setTextSpeed(int speed);
    void setRainbowSpeed(int speed);
    void setBrightness(int brightness);
    void setScreenMode(int mode);
    void setSensorBgEnabled(bool enabled);
    void setCustomText(const String& text);

private:
    Preferences preferences_;

    // Runtime state — initialized from NVS on load()
    time_t  targetEpoch_   = 0;
    bool    ledEnabled_    = true;
    bool    splashEnabled_ = true;
    bool    rainbowEnabled_ = true;
    bool    showSeconds_   = true;
    int     textSpeed_     = 4;
    int     rainbowSpeed_  = 5;
    int     brightnessVal_ = 60;
    int     screenMode_      = 0;     // 0 = Countdown, 1 = Sensor
    bool    sensorBgEnabled_ = true;  // Background gradient
    String  customText_      = "Co len!";
};
