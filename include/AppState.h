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
#include <freertos/semphr.h>

#include <atomic>

class AppState {
   public:
	void lock() const {
		if (mutex_) xSemaphoreTake(mutex_, portMAX_DELAY);
	}
	void unlock() const {
		if (mutex_) xSemaphoreGive(mutex_);
	}
	/**
	 * @brief Load all settings from NVS flash. Call once in setup().
	 */
	void load();

	// ─── Getters ─────────────────────────────────────────────────
	bool consumeSplashPlayRequest() {
		return playSplashReq_.exchange(false);
	}
	void requestSplashPlay() {
		playSplashReq_.store(true);
	}

	time_t getTargetEpoch() const {
		return targetEpoch_;
	}
	bool isLedEnabled() const {
		return ledEnabled_;
	}
	bool isSplashEnabled() const {
		return splashEnabled_;
	}
	const String& getSplashText() const {
		return splashText_;
	}
	bool isRainbowEnabled() const {
		return rainbowEnabled_;
	}
	bool isShowSecondsCountdownEnabled() const {
		return showSecondsCountdown_;
	}
	bool isShowSecondsDateTimeEnabled() const {
		return showSecondsDateTime_;
	}
	int getTextSpeed() const {
		return textSpeed_;
	}
	int getRainbowSpeed() const {
		return rainbowSpeed_;
	}
	int getBrightness() const {
		return brightnessVal_;
	}
	int getScreenMode() const {
		return screenMode_;
	}
	bool isSensorBgEnabled() const {
		return sensorBgEnabled_;
	}
	bool isShowMessageEnabled() const {
		return showMessageEnabled_;
	}
	bool isWifiIconEnabled() const {
		return wifiIconEnabled_;
	}
	const String& getCustomText() const {
		return customText_;
	}
	const String& getTextPanelContent() const {
		return textPanelContent_;
	}
	int getTextPanelSpeed() const {
		return textPanelSpeed_;
	}
	int getTextPanelColor() const {
		return textPanelColor_;
	}
	bool isTextPanelScrollEnabled() const {
		return textPanelScroll_;
	}
	int getCdDaysColor() const {
		return cdDaysColor_;
	}
	int getCdTimeColor() const {
		return cdTimeColor_;
	}

	// Date formatting
	bool isShowYearEnabled() const {
		return showYear_;
	}
	bool isShortYearEnabled() const {
		return shortYear_;
	}
	int getDateSeparator() const {
		return dateSeparator_;
	}

	// WiFi
	const String& getStaSSID() const {
		return staSSID_;
	}
	const String& getStaPass() const {
		return staPass_;
	}
	const String& getApSSID() const {
		return apSSID_;
	}
	const String& getApPass() const {
		return apPass_;
	}

	// Auto-Cycle
	bool isAutoCycleEnabled() const {
		return autoCycle_;
	}
	int getTimer0_Countdown() const {
		return timer0_;
	}
	int getTimer1_Sensor() const {
		return timer1_;
	}
	int getTimer2_DateTime() const {
		return timer2_;
	}
	int getTimer3_Text() const {
		return timer3_;
	}

	const String& getFwSchedules() const {
		return fwSchedules_;
	}
	const String& getCycleSequence() const {
		return cycleSequence_;
	}

	struct FwSchedule {
		int hour	 = -1;
		int minute	 = 0;
		int duration = 0;
	};
	const FwSchedule* getParsedFwSchedules() const {
		return fwSchedulesParsed_;
	}
	const int* getParsedCycleSequence(int& len) const {
		len = cycleSequenceLen_;
		return cycleSequenceParsed_;
	}

	// ─── Setters (auto-persist to NVS) ───────────────────────────
	void setTargetEpoch(time_t epoch);
	void setLedEnabled(bool enabled);
	void setSplashEnabled(bool enabled);
	void setSplashText(const String& text);
	void setRainbowEnabled(bool enabled);
	void setShowSecondsCountdownEnabled(bool enabled);
	void setShowSecondsDateTimeEnabled(bool enabled);
	void setTextSpeed(int speed);
	void setRainbowSpeed(int speed);
	void setBrightness(int brightness);
	void setScreenMode(int mode);
	void setScreenModeVolatile(int mode) {
		screenMode_ = mode;
	}  // Does not save to flash (Anti-wear for auto-cycle)
	void setSensorBgEnabled(bool enabled);
	void setShowMessageEnabled(bool enabled);
	void setWifiIconEnabled(bool enabled);
	void setCustomText(const String& text);
	void setTextPanelContent(const String& text);
	void setTextPanelSpeed(int speed);
	void setTextPanelColor(int color);
	void setTextPanelScrollEnabled(bool enabled);
	void setCdDaysColor(int color);
	void setCdTimeColor(int color);

	void setShowYearEnabled(bool enabled);
	void setShortYearEnabled(bool enabled);
	void setDateSeparator(int sep);

	void setStaSSID(const String& ssid);
	void setStaPass(const String& pass);
	void setApSSID(const String& ssid);
	void setApPass(const String& pass);

	void setAutoCycleEnabled(bool enabled);
	void setTimer0_Countdown(int t);
	void setTimer1_Sensor(int t);
	void setTimer2_DateTime(int t);
	void setTimer3_Text(int t);

	void setFwSchedules(const String& scheds);
	void setCycleSequence(const String& seq);

   private:
	std::atomic<bool> playSplashReq_{false};
	SemaphoreHandle_t mutex_ = NULL;
	Preferences preferences_;

	// Runtime state — initialized from NVS on load()
	time_t targetEpoch_		   = 0;
	bool ledEnabled_		   = true;
	bool splashEnabled_		   = true;
	String splashText_		   = "12A6";
	bool rainbowEnabled_	   = true;
	bool showSecondsCountdown_ = true;
	bool showSecondsDateTime_  = true;
	int textSpeed_			   = 4;
	int rainbowSpeed_		   = 5;
	int brightnessVal_		   = 60;
	int screenMode_ = 0;  // 0 = Countdown, 1 = Sensor, 2 = Date/Time+Sensor
	bool sensorBgEnabled_	 = true;  // Background gradient
	bool showMessageEnabled_ = true;
	bool wifiIconEnabled_	 = true;
	String customText_		 = "Co len!";
	String textPanelContent_ = "XIN CHAO";
	int textPanelSpeed_		 = 5;
	int textPanelColor_		 = 0;
	bool textPanelScroll_	 = true;
	int cdDaysColor_		 = 3;  // Yellow
	int cdTimeColor_		 = 4;  // Cyan

	bool showYear_	   = true;
	bool shortYear_	   = false;
	int dateSeparator_ = 0;

	String staSSID_ = "Belvedere";
	String staPass_ = "123789456@";
	String apSSID_	= "THPT_QG_Config";
	String apPass_	= "12345678@";

	bool autoCycle_ = false;
	int timer0_		= 10;
	int timer1_		= 5;
	int timer2_		= 10;
	int timer3_		= 20;

	String fwSchedules_	  = "";
	String cycleSequence_ = "";

	FwSchedule fwSchedulesParsed_[10];
	int cycleSequenceParsed_[10];
	int cycleSequenceLen_ = 0;

	void parseFwSchedules();
	void parseCycleSequence();
};
