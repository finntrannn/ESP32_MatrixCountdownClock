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
	bool isOpenMeteoEnabled() const {
		return openMeteoEnabled_;
	}
	float getWeatherLat() const {
		return weatherLat_;
	}
	float getWeatherLon() const {
		return weatherLon_;
	}
	float getRealTemp() const {
		return realTemp_;
	}
	float getRealHumi() const {
		return realHumi_;
	}
	bool isRealDataValid() const {
		return realDataValid_;
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
	int getTextPanelSize() const {
		return textPanelSize_;
	}
	const String& getTextPanelColor() const {
		return textPanelColor_;
	}
	bool isTextPanelScrollEnabled() const {
		return textPanelScroll_;
	}
	const String& getCdDaysColor() const {
		return cdDaysColor_;
	}
	const String& getCdTimeColor() const {
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
	int getTimer4_Spinner() const {
		return timer4_;
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

	// ─── Spinner Getters ─────────────────────────────────────────
	int getSpinRangeMin() const {
		return spinRangeMin_;
	}
	int getSpinRangeMax() const {
		return spinRangeMax_;
	}
	int getSpinDuration() const {
		return spinDuration_;
	}
	const String& getExcludedNumbers() const {
		return excludedNumbers_;
	}
	bool consumeSpinRequest() {
		return spinRequest_.exchange(false);
	}
	void requestSpin() {
		spinRequest_.store(true);
	}
	bool consumeSpinResetRequest() {
		return spinResetRequest_.exchange(false);
	}
	void requestSpinReset() {
		spinResetRequest_.store(true);
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
	void setOpenMeteoEnabled(bool enabled);
	void setWeatherLat(float lat);
	void setWeatherLon(float lon);
	void setRealWeatherData(float temp, float humi, bool valid) {
		lock();
		realTemp_ = temp;
		realHumi_ = humi;
		realDataValid_ = valid;
		unlock();
	}
	void setShowMessageEnabled(bool enabled);
	void setWifiIconEnabled(bool enabled);
	void setCustomText(const String& text);
	void setTextPanelContent(const String& text);
	void setTextPanelSpeed(int speed);
	void setTextPanelSize(int size);
	void setTextPanelColor(const String& color);
	void setTextPanelScrollEnabled(bool enabled);
	void setCdDaysColor(const String& color);
	void setCdTimeColor(const String& color);

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
	void setTimer4_Spinner(int t);

	void setFwSchedules(const String& scheds);
	void setCycleSequence(const String& seq);

	// Spinner setters
	void setSpinRangeMin(int val);
	void setSpinRangeMax(int val);
	void setSpinDuration(int val);
	void setExcludedNumbers(const String& nums);

   private:
	std::atomic<bool> playSplashReq_{false};
	std::atomic<bool> spinRequest_{false};
	std::atomic<bool> spinResetRequest_{false};
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
	int screenMode_ = 0;  // 0=Countdown, 1=Sensor, 2=DateTime, 3=Text, 4=Spinner
	bool sensorBgEnabled_	 = true;
	bool openMeteoEnabled_   = false;
	float weatherLat_        = 21.0285f;
	float weatherLon_        = 105.8542f;
	float realTemp_          = 0.0f;
	float realHumi_          = 0.0f;
	bool realDataValid_      = false;
	bool showMessageEnabled_ = true;
	bool wifiIconEnabled_	 = true;
	String customText_		 = "Co len!";
	String textPanelContent_ = "XIN CHAO";
	int textPanelSpeed_		 = 5;
	int textPanelSize_		 = 1;
	String textPanelColor_	 = "#00FF00";
	bool textPanelScroll_	 = true;
	String cdDaysColor_		 = "#FFFF00";
	String cdTimeColor_		 = "#00FFFF";

	bool showYear_	   = true;
	bool shortYear_	   = false;
	int dateSeparator_ = 0;

	String staSSID_ = "YOUR_WIFI_SSID";
	String staPass_ = "YOUR_WIFI_PASSWORD";
	String apSSID_	= "THPT_QG_Config";
	String apPass_	= "12345678";

	bool autoCycle_ = false;
	int timer0_		= 10;
	int timer1_		= 5;
	int timer2_		= 10;
	int timer3_		= 20;
	int timer4_		= 10;

	String fwSchedules_	  = "";
	String cycleSequence_ = "";

	FwSchedule fwSchedulesParsed_[10];
	int cycleSequenceParsed_[10];
	int cycleSequenceLen_ = 0;

	// Spinner
	int spinRangeMin_		= 1;
	int spinRangeMax_		= 48;
	int spinDuration_		= 5;
	String excludedNumbers_ = "";

	void parseFwSchedules();
	void parseCycleSequence();
};
