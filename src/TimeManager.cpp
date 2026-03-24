/**
 * @file TimeManager.cpp
 * @brief Implementation of TimeManager for NTP and RTC.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "TimeManager.h"

#include <Wire.h>
#include <esp_sntp.h>

#include "Config.h"

RTC_DS3231 TimeManager::rtc;
bool TimeManager::rtcFound = false;

void TimeManager::timeSyncCallback(struct timeval *tv) {
	if (rtcFound) {
		time_t now = tv->tv_sec;
		struct tm timeinfo;
		gmtime_r(&now, &timeinfo);

		// Lưu giờ UTC chuẩn vào mạch RTC DS3231
		rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
							timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min,
							timeinfo.tm_sec));

		Serial.println("NTP Time Synced! Updated DS3231 RTC accurately.");
	}
}

void TimeManager::begin() {
	Serial.println("Initializing TimeManager...");

	// 1. Khởi tạo I2C cho RTC
	Wire.begin(RTCConfig::kPinSDA, RTCConfig::kPinSCL);
	if (rtc.begin(&Wire)) {
		rtcFound = true;
		if (rtc.lostPower()) {
			Serial.println(
				"RTC lost power! Defaulting to compile time before NTP sync.");
			rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		}

		// 2. Chuyển thời gian khởi động từ RTC vào bộ đếm nội bộ EPS32
		// (micro-giây)
		DateTime nowRtc = rtc.now();
		struct timeval tv;
		tv.tv_sec  = nowRtc.unixtime();
		tv.tv_usec = 0;
		settimeofday(&tv, nullptr);

		Serial.println("System time temporarily populated from DS3231 RTC.");
	} else {
		Serial.println("WARNING: Couldn't find RTC DS3231.");
	}

	// 3. Đăng ký hàm gọi lại (callback) SNTP để tự động cập nhật lại RTC khi có
	// mạng
	sntp_set_time_sync_notification_cb(timeSyncCallback);

	// 4. Bật quá trình configTime nội bộ ESP32 để tìm NTP
	configTime(NtpConfig::kGmtOffsetSec, NtpConfig::kDaylightOffset,
			   NtpConfig::kServer1, NtpConfig::kServer2);
}

time_t TimeManager::getCurrentTime() const {
	return time(nullptr);
}

bool TimeManager::isTimeSynced() const {
	// Nếu Unix time > năm 2000 (~946684800) thì hệ thống đã nhận giờ thành
	// công.
	return time(nullptr) > 946684800;
}
