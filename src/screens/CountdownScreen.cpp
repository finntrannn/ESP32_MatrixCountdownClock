/**
 * @file CountdownScreen.cpp
 * @brief Renders the THPT QG Countdown interface.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/CountdownScreen.h"
#include "Config.h"

void CountdownScreen::draw(float dt, DisplayManager& display, NetworkManager& network, const AppState& appState) {
    auto *panel = display.getPanel();

    // ── 1. WiFi Status Indicator (bottom-right pixel) ────────────
    uint16_t statusColor = network.isConnected()
                               ? panel->color565(0, 255, 0)
                               : panel->color565(255, 128, 0);
    panel->fillRect(62, 30, 1, 1, statusColor);

    // ── 2. Current Time (top-center mini clock) ──────────────────
    time_t now = network.getCurrentTime();
    if (network.isTimeSynced())
    {
        struct tm *timeinfo = localtime(&now);
        char timeStr[6];
        sprintf(timeStr, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
        display.drawMiniString(23, 1, timeStr, panel->color565(200, 200, 200));
    }
    else
    {
        display.drawMiniString(23, 1, "--:--", panel->color565(255, 0, 0));
    }

    // ── 3. Countdown (days + HH:MM:SS) ──────────────────────────
    time_t now2 = network.getCurrentTime();
    long diff = appState.getTargetEpoch() - now2;
    if (diff < 0 || !network.isTimeSynced())
        diff = 0;

    long days = diff / 86400;
    long hours = (diff % 86400) / 3600;
    long mins = (diff % 3600) / 60;
    long secs = diff % 60;

    panel->setTextSize(1);
    panel->setTextWrap(false);

    // Days row
    char dayStr[16];
    sprintf(dayStr, "%03d NGAY", (int)days);
    panel->setTextColor(panel->color565(255, 255, 0));
    panel->setCursor(8, 8);
    panel->print(dayStr);
    // Accent mark for "NGÀY"
    panel->drawLine(45, 5, 46, 6, panel->color565(255, 255, 0));

    // Hours:Minutes:Seconds row
    char hmsStr[16];
    if (appState.isShowSecondsEnabled())
    {
        sprintf(hmsStr, "%02d:%02d:%02d", (int)hours, (int)mins, (int)secs);
        panel->setCursor(8, 16);
    }
    else
    {
        sprintf(hmsStr, "%02d:%02d", (int)hours, (int)mins);
        panel->setCursor(17, 16);
    }
    panel->setTextColor(panel->color565(0, 255, 255));
    panel->print(hmsStr);

    // ── 4. Custom Scrolling Text ─────────────────────────────────
    panel->setTextColor(panel->color565(50, 255, 50));
    const String &customText = appState.getCustomText();
    int textWidth = customText.length() * 6;

    if (textWidth <= Panel::kResX)
    {
        int centerX = (Panel::kResX - textWidth) / 2;
        panel->setCursor(centerX, 24);
        panel->print(customText);
    }
    else
    {
        panel->setCursor((int)textX_, 24);
        panel->print(customText);

        textX_ -= (appState.getTextSpeed() * 8.0f * dt);
        if (textX_ < -textWidth)
        {
            textX_ = static_cast<float>(Panel::kResX);
        }
    }

    // ── 5. Rainbow Border ────────────────────────────────────────
    if (appState.isRainbowEnabled())
    {
        display.drawRainbowBorder(static_cast<uint16_t>(borderFloatOffset_));

        borderFloatOffset_ += appState.getRainbowSpeed() * 100.0f * dt;
        if (borderFloatOffset_ >= 256.0f)
        {
            borderFloatOffset_ -= 256.0f;
        }
    }
}
