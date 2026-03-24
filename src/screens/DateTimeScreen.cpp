/**
 * @file DateTimeScreen.cpp
 * @brief Renders only the Date and Time interface.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/DateTimeScreen.h"
#include "Config.h"

void DateTimeScreen::draw(float dt, DisplayManager& display, TimeManager& timeManager, const AppState& appState) {
    auto *panel = display.getPanel();

    // ── 1. Background gradient ──────────────────────────────────────
    if (appState.isSensorBgEnabled())
    {
        for (int y = 0; y < Panel::kResY; y++)
        {
            uint8_t bgBright = 2 + (y * 4) / Panel::kResY;
            panel->drawFastHLine(0, y, Panel::kResX, panel->color565(bgBright, bgBright, bgBright + 3));
        }
    }

    panel->setTextSize(1);
    panel->setTextWrap(false);

    // ── 2. Time & Date ──────────────────────────────────────────────
    time_t now = timeManager.getCurrentTime();
    if (timeManager.isTimeSynced())
    {
        struct tm *timeinfo = localtime(&now);
        
        // Time (HH:MM:SS) - Cyan color
        char timeStr[16];
        if (appState.isShowSecondsDateTimeEnabled()) {
            sprintf(timeStr, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        } else {
            sprintf(timeStr, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
        }
        int timeWidth = strlen(timeStr) * 6;
        int timeX = (Panel::kResX - timeWidth) / 2;
        panel->setTextColor(panel->color565(0, 255, 255));
        
        // Đẩy thời gian lùi xuống vài pixel
        int timeY = 8;
        panel->setCursor(timeX, timeY);
        panel->print(timeStr);

        // Date Format Selection - Yellow color
        char dateStr[64];
        char sep = '/';
        int sepIdx = appState.getDateSeparator();
        if (sepIdx == 1) sep = '.';
        if (sepIdx == 2) sep = '-';

        if (appState.isShowYearEnabled()) {
            if (appState.isShortYearEnabled()) {
                snprintf(dateStr, sizeof(dateStr), "%02d%c%02d%c%02d", timeinfo->tm_mday, sep, timeinfo->tm_mon + 1, sep, (timeinfo->tm_year + 1900) % 100);
            } else {
                snprintf(dateStr, sizeof(dateStr), "%02d%c%02d%c%04d", timeinfo->tm_mday, sep, timeinfo->tm_mon + 1, sep, timeinfo->tm_year + 1900);
            }
        } else {
            snprintf(dateStr, sizeof(dateStr), "%02d%c%02d", timeinfo->tm_mday, sep, timeinfo->tm_mon + 1);
        }
        
        int dateWidth = strlen(dateStr) * 6;
        int dateX = (Panel::kResX - dateWidth) / 2;
        panel->setTextColor(panel->color565(255, 255, 0));
        
        // Đẩy ngày xuống tương ứng
        int dateY = 18;
        panel->setCursor(dateX, dateY);
        panel->print(dateStr);
    }
    else
    {
        panel->setTextColor(panel->color565(255, 0, 0));
        panel->setCursor(14, 12);
        panel->print("--:--");
    }
}
