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
	if (mutex_ == NULL) mutex_ = xSemaphoreCreateMutex();
	preferences_.begin(NvsKeys::kNamespace, false);

	targetEpoch_ =
		preferences_.getUInt(NvsKeys::kTargetEpoch, Defaults::kTargetEpoch);
	ledEnabled_ =
		preferences_.getBool(NvsKeys::kLedState, Defaults::kLedEnabled);
	splashEnabled_ =
		preferences_.getBool(NvsKeys::kSplashEn, Defaults::kSplashEnabled);
	customText_ =
		preferences_.getString(NvsKeys::kCustomText, Defaults::kCustomText);
	rainbowEnabled_ =
		preferences_.getBool(NvsKeys::kRainbow, Defaults::kRainbowEnabled);
	showSecondsCountdown_ = preferences_.getBool(
		NvsKeys::kShowSecsCD, Defaults::kShowSecondsCountdown);
	showSecondsDateTime_ = preferences_.getBool(NvsKeys::kShowSecsDT,
												Defaults::kShowSecondsDateTime);
	textSpeed_ = preferences_.getInt(NvsKeys::kTextSpeed, Defaults::kTextSpeed);
	rainbowSpeed_ =
		preferences_.getInt(NvsKeys::kRainbowSpeed, Defaults::kRainbowSpeed);
	brightnessVal_ =
		preferences_.getInt(NvsKeys::kBrightness, Defaults::kBrightness);
	screenMode_ =
		preferences_.getInt(NvsKeys::kScreenMode, Defaults::kScreenMode);
	sensorBgEnabled_ =
		preferences_.getBool(NvsKeys::kSensorBg, Defaults::kSensorBgEnabled);
	showMessageEnabled_ =
		preferences_.getBool(NvsKeys::kShowMessage, Defaults::kShowMessage);
	wifiIconEnabled_ =
		preferences_.getBool(NvsKeys::kWifiIcon, Defaults::kWifiIcon);
	textPanelContent_ = preferences_.getString(NvsKeys::kTxtContent,
											   Defaults::kTextPanelContent);
	textPanelSpeed_ =
		preferences_.getInt(NvsKeys::kTxtSpeed, Defaults::kTextPanelSpeed);
	textPanelColor_ =
		preferences_.getInt(NvsKeys::kTxtColor, Defaults::kTextPanelColor);
	textPanelScroll_ =
		preferences_.getBool(NvsKeys::kTxtScroll, Defaults::kTextPanelScroll);
	cdDaysColor_ =
		preferences_.getInt(NvsKeys::kCdDaysClr, Defaults::kCdDaysColor);
	cdTimeColor_ =
		preferences_.getInt(NvsKeys::kCdTimeClr, Defaults::kCdTimeColor);

	showYear_ = preferences_.getBool(NvsKeys::kShowYear, Defaults::kShowYear);
	shortYear_ =
		preferences_.getBool(NvsKeys::kShortYear, Defaults::kShortYear);
	dateSeparator_ =
		preferences_.getInt(NvsKeys::kDateSep, Defaults::kDateSeparator);

	staSSID_ = preferences_.getString(NvsKeys::kStaSSID, Defaults::kStaSSID);
	staPass_ = preferences_.getString(NvsKeys::kStaPass, Defaults::kStaPass);
	apSSID_	 = preferences_.getString(NvsKeys::kApSSID, Defaults::kApSSID);
	apPass_	 = preferences_.getString(NvsKeys::kApPass, Defaults::kApPass);

	autoCycle_ =
		preferences_.getBool(NvsKeys::kAutoCycle, Defaults::kAutoCycle);
	timer0_ = preferences_.getInt(NvsKeys::kTimer0, Defaults::kTimer0);
	timer1_ = preferences_.getInt(NvsKeys::kTimer1, Defaults::kTimer1);
	timer2_ = preferences_.getInt(NvsKeys::kTimer2, Defaults::kTimer2);
	timer3_ = preferences_.getInt(NvsKeys::kTimer3, Defaults::kTimer3);

	fwSchedules_ =
		preferences_.getString(NvsKeys::kFwScheds, Defaults::kFwScheds);
	cycleSequence_ =
		preferences_.getString(NvsKeys::kCycleSeq, Defaults::kCycleSeq);

	parseFwSchedules();
	parseCycleSequence();
}

void AppState::parseFwSchedules() {
	for (int i = 0; i < 10; i++) fwSchedulesParsed_[i].hour = -1;
	int idx		 = 0;
	int startIdx = 0;
	while (startIdx < (int)fwSchedules_.length() && idx < 10) {
		int semiIdx = fwSchedules_.indexOf(';', startIdx);
		if (semiIdx == -1) semiIdx = fwSchedules_.length();
		String slot = fwSchedules_.substring(startIdx, semiIdx);
		startIdx	= semiIdx + 1;

		int colonIdx = slot.indexOf(':');
		int commaIdx = slot.indexOf(',');
		if (colonIdx > 0 && commaIdx > colonIdx) {
			fwSchedulesParsed_[idx].hour = slot.substring(0, colonIdx).toInt();
			fwSchedulesParsed_[idx].minute =
				slot.substring(colonIdx + 1, commaIdx).toInt();
			fwSchedulesParsed_[idx].duration =
				slot.substring(commaIdx + 1).toInt();
			idx++;
		}
	}
}

void AppState::parseCycleSequence() {
	cycleSequenceLen_ = 0;
	int startIdx	  = 0;
	while (startIdx < (int)cycleSequence_.length() && cycleSequenceLen_ < 10) {
		int commaIdx = cycleSequence_.indexOf(',', startIdx);
		if (commaIdx == -1) commaIdx = cycleSequence_.length();
		String modeStr = cycleSequence_.substring(startIdx, commaIdx);
		startIdx	   = commaIdx + 1;
		if (modeStr.length() > 0) {
			cycleSequenceParsed_[cycleSequenceLen_++] = modeStr.toInt();
		}
	}
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

void AppState::setShowSecondsCountdownEnabled(bool enabled) {
	showSecondsCountdown_ = enabled;
	preferences_.putBool(NvsKeys::kShowSecsCD, enabled);
}

void AppState::setShowSecondsDateTimeEnabled(bool enabled) {
	showSecondsDateTime_ = enabled;
	preferences_.putBool(NvsKeys::kShowSecsDT, enabled);
}

void AppState::setTextSpeed(int speed) {
	if (speed < 1) speed = 1;
	if (speed > 10) speed = 10;
	textSpeed_ = speed;
	preferences_.putInt(NvsKeys::kTextSpeed, speed);
}

void AppState::setRainbowSpeed(int speed) {
	if (speed < 1) speed = 1;
	if (speed > 10) speed = 10;
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
	if (mode < 0) mode = 0;
	if (mode > 3) mode = 3;
	screenMode_ = mode;
	preferences_.putInt(NvsKeys::kScreenMode, mode);
}

void AppState::setSensorBgEnabled(bool enabled) {
	sensorBgEnabled_ = enabled;
	preferences_.putBool(NvsKeys::kSensorBg, enabled);
}

void AppState::setWifiIconEnabled(bool enabled) {
	wifiIconEnabled_ = enabled;
	preferences_.putBool(NvsKeys::kWifiIcon, enabled);
}

void AppState::setShowMessageEnabled(bool enabled) {
	showMessageEnabled_ = enabled;
	preferences_.putBool(NvsKeys::kShowMessage, enabled);
}

void AppState::setCustomText(const String& text) {
	customText_ = text;
	preferences_.putString(NvsKeys::kCustomText, text);
}

void AppState::setTextPanelContent(const String& text) {
	textPanelContent_ = text;
	preferences_.putString(NvsKeys::kTxtContent, text);
}

void AppState::setTextPanelSpeed(int speed) {
	if (speed < 1) speed = 1;
	if (speed > 10) speed = 10;
	textPanelSpeed_ = speed;
	preferences_.putInt(NvsKeys::kTxtSpeed, speed);
}

void AppState::setTextPanelColor(int color) {
	if (color < 0) color = 0;
	if (color > 6) color = 6;
	textPanelColor_ = color;
	preferences_.putInt(NvsKeys::kTxtColor, color);
}

void AppState::setTextPanelScrollEnabled(bool enabled) {
	textPanelScroll_ = enabled;
	preferences_.putBool(NvsKeys::kTxtScroll, enabled);
}

void AppState::setCdDaysColor(int color) {
	if (color < 0) color = 0;
	if (color > 6) color = 6;
	cdDaysColor_ = color;
	preferences_.putInt(NvsKeys::kCdDaysClr, color);
}

void AppState::setCdTimeColor(int color) {
	if (color < 0) color = 0;
	if (color > 6) color = 6;
	cdTimeColor_ = color;
	preferences_.putInt(NvsKeys::kCdTimeClr, color);
}

void AppState::setShowYearEnabled(bool enabled) {
	showYear_ = enabled;
	preferences_.putBool(NvsKeys::kShowYear, enabled);
}

void AppState::setShortYearEnabled(bool enabled) {
	shortYear_ = enabled;
	preferences_.putBool(NvsKeys::kShortYear, enabled);
}

void AppState::setDateSeparator(int sep) {
	dateSeparator_ = sep;
	preferences_.putInt(NvsKeys::kDateSep, sep);
}

void AppState::setStaSSID(const String& ssid) {
	staSSID_ = ssid;
	preferences_.putString(NvsKeys::kStaSSID, ssid);
}

void AppState::setStaPass(const String& pass) {
	staPass_ = pass;
	preferences_.putString(NvsKeys::kStaPass, pass);
}

void AppState::setApSSID(const String& ssid) {
	apSSID_ = ssid;
	preferences_.putString(NvsKeys::kApSSID, ssid);
}

void AppState::setApPass(const String& pass) {
	apPass_ = pass;
	preferences_.putString(NvsKeys::kApPass, pass);
}

void AppState::setAutoCycleEnabled(bool enabled) {
	autoCycle_ = enabled;
	preferences_.putBool(NvsKeys::kAutoCycle, enabled);
}

void AppState::setTimer0_Countdown(int t) {
	if (t < 0) t = 0;
	timer0_ = t;
	preferences_.putInt(NvsKeys::kTimer0, t);
}

void AppState::setTimer1_Sensor(int t) {
	if (t < 0) t = 0;
	timer1_ = t;
	preferences_.putInt(NvsKeys::kTimer1, t);
}

void AppState::setTimer2_DateTime(int t) {
	if (t < 0) t = 0;
	timer2_ = t;
	preferences_.putInt(NvsKeys::kTimer2, t);
}

void AppState::setTimer3_Text(int t) {
	if (t < 0) t = 0;
	timer3_ = t;
	preferences_.putInt(NvsKeys::kTimer3, t);
}

void AppState::setFwSchedules(const String& scheds) {
	fwSchedules_ = scheds;
	preferences_.putString(NvsKeys::kFwScheds, scheds);
	parseFwSchedules();
}

void AppState::setCycleSequence(const String& seq) {
	cycleSequence_ = seq;
	preferences_.putString(NvsKeys::kCycleSeq, seq);
	parseCycleSequence();
}
