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
constexpr int kResX	 = 64;
constexpr int kResY	 = 32;
constexpr int kChain = 1;
}  // namespace Panel

// ─── DHT11 Sensor ────────────────────────────────────────────────────
namespace DHTConfig {
constexpr uint8_t kPin = 32;  // GPIO pin connected to DHT11 data
}

// ─── RTC DS3231 ──────────────────────────────────────────────────────
namespace RTCConfig {
constexpr uint8_t kPinSDA = 21;	 // Mặc định I2C SDA của ESP32
constexpr uint8_t kPinSCL = 22;	 // Mặc định I2C SCL của ESP32
}  // namespace RTCConfig

// ─── Default Settings ───────────────────────────────────────────────
namespace Defaults {
constexpr uint32_t kTargetEpoch		 = 1782320400;	// 25/06/2026 00:00 GMT+7
constexpr bool kLedEnabled			 = true;
constexpr bool kSplashEnabled		 = true;
constexpr const char *kSplashText	 = "12A6";
constexpr bool kRainbowEnabled		 = true;
constexpr int kTextSpeed			 = 4;	  // 1 (slow) – 10 (fast)
constexpr int kRainbowSpeed			 = 5;	  // 1 (slow) – 10 (fast)
constexpr bool kShowSecondsCountdown = true;  // Show seconds in countdown
constexpr bool kShowSecondsDateTime	 = true;  // Show seconds in date/time
constexpr int kBrightness			 = 60;	  // 1 (dim) – 255 (max)
constexpr const char *kCustomText	 = "Co len!";
constexpr int kScreenMode =
	0;	// 0 = Countdown, 1 = Sensor, 2 = Date/Time, 3 = Text Matrix
constexpr bool kSensorBgEnabled = true;	 // Background gradient on sensor screen
constexpr bool kShowMessage		= true;	 // Show motivational message
constexpr bool kWifiIcon		= true;	 // Show WiFi status indicator pixel
// Text Matrix Settings
constexpr const char *kTextPanelContent = "XIN CHAO";
constexpr int kTextPanelSpeed			= 5;
constexpr const char *kTextPanelColor	= "#00FF00";	 // Green hex
constexpr int kTextPanelSize			= 1;
constexpr bool kTextPanelScroll			= true;

// Countdown screen colors (hex)
constexpr const char *kCdDaysColor = "#FFFF00";	 // Yellow
constexpr const char *kCdTimeColor = "#00FFFF";	 // Cyan

// Random Spinner
constexpr int kSpinRangeMin	 = 1;
constexpr int kSpinRangeMax	 = 48;
constexpr int kSpinDuration	 = 5;	  // seconds
constexpr int kTimer4		 = 10;

// Date formatting
constexpr bool kShowYear	 = true;
constexpr bool kShortYear	 = false;
constexpr int kDateSeparator = 0;  // 0: '/', 1: '.', 2: '-'

// WiFi
constexpr const char *kStaSSID = "YOUR_WIFI_SSID";
constexpr const char *kStaPass = "YOUR_WIFI_PASSWORD";
constexpr const char *kApSSID  = "THPT_QG_Config";
constexpr const char *kApPass  = "12345678";

// Auto-Cycle
constexpr bool kAutoCycle = false;
constexpr int kTimer0	  = 10;
constexpr int kTimer1	  = 5;
constexpr int kTimer2	  = 10;
constexpr int kTimer3	  = 20;

constexpr const char *kFwScheds = "10:35,5;08:40,5;16:50,10";
constexpr const char *kCycleSeq = "0,1,2,3";
}  // namespace Defaults

// ─── NVS Preference Keys ────────────────────────────────────────────
namespace NvsKeys {
constexpr const char *kNamespace	= "clock_app";
constexpr const char *kTargetEpoch	= "targetEpoch";
constexpr const char *kLedState		= "ledState";
constexpr const char *kSplashEn		= "splashEn";
constexpr const char *kSplashText	= "splashTxt";
constexpr const char *kCustomText	= "customText";
constexpr const char *kRainbow		= "rainbow";
constexpr const char *kTextSpeed	= "textSpd";
constexpr const char *kRainbowSpeed = "rbSpd";
constexpr const char *kShowSecsCD	= "showSecsCD";
constexpr const char *kShowSecsDT	= "showSecsDT";
constexpr const char *kBrightness	= "brightness";
constexpr const char *kScreenMode	= "screenMode";
constexpr const char *kSensorBg		= "sensBg";
constexpr const char *kShowMessage	= "showMsg";
constexpr const char *kTxtContent	= "txtContent";
constexpr const char *kTxtSpeed		= "txtSpeed";
constexpr const char *kTxtSize		= "txtSize";
constexpr const char *kTxtColor		= "txtColor";
constexpr const char *kTxtScroll	= "txtScroll";
constexpr const char *kCdDaysClr	= "cdDaysClr";
constexpr const char *kCdTimeClr	= "cdTimeClr";

constexpr const char *kShowYear	 = "showYear";
constexpr const char *kShortYear = "shortYear";
constexpr const char *kDateSep	 = "dateSep";

constexpr const char *kStaSSID = "staSSID";
constexpr const char *kStaPass = "staPass";
constexpr const char *kApSSID  = "apSSID";
constexpr const char *kApPass  = "apPass";

constexpr const char *kAutoCycle = "autoCycle";
constexpr const char *kTimer0	 = "timer0";
constexpr const char *kTimer1	 = "timer1";
constexpr const char *kTimer2	 = "timer2";
constexpr const char *kTimer3	 = "timer3";
constexpr const char *kTimer4	 = "timer4";
constexpr const char *kFwScheds	 = "fwScheds";
constexpr const char *kCycleSeq	 = "cycleSeq";
constexpr const char *kWifiIcon	 = "wifiIcon";

// Spinner
constexpr const char *kSpinMin	 = "spinMin";
constexpr const char *kSpinMax	 = "spinMax";
constexpr const char *kSpinDur	 = "spinDur";
constexpr const char *kSpinExcl	 = "spinExcl";
}  // namespace NvsKeys

// ─── NTP Configuration ──────────────────────────────────────────────
namespace NtpConfig {
constexpr long kGmtOffsetSec   = 7 * 3600;	// GMT+7
constexpr int kDaylightOffset  = 0;
constexpr const char *kServer1 = "pool.ntp.org";
constexpr const char *kServer2 = "time.nist.gov";
}  // namespace NtpConfig
