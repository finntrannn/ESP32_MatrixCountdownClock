/**
 * @file TimeManager.h
 * @brief Manages NTP and RTC (DS3231) time synchronization.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <ctime>
#include <RTClib.h>

class TimeManager {
public:
    /**
     * @brief Khởi tạo RTC và thiết lập NTP.
     */
    void begin();

    /** @brief Lấy thời gian hiện tại từ hệ thống (đã được đồng bộ NTP/RTC). */
    time_t getCurrentTime() const;

    /** @brief Trả về true nếu thời gian đã được thiết lập hợp lý. */
    bool isTimeSynced() const;

private:
    static RTC_DS3231 rtc;
    static bool rtcFound;

    /** @brief Callback được gọi bởi SNTP khi lấy được giờ chuẩn từ mạng. */
    static void timeSyncCallback(struct timeval *tv);
};
