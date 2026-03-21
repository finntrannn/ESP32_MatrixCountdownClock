/**
 * @file SensorScreen.cpp
 * @brief Renders the DHT11 Temperature & Humidity interface.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/SensorScreen.h"
#include "Config.h"

void SensorScreen::resetAnimations() {
    sensorBarSmooth_t_ = 0.0f;
    sensorBarSmooth_h_ = 0.0f;
}

void SensorScreen::draw(float dt, DisplayManager& display, const DHTSensor& dhtSensor, const AppState& appState) {
    auto *panel = display.getPanel();

    // ── Background gradient (toggleable) ─────────────────────────
    if (appState.isSensorBgEnabled())
    {
        for (int y = 0; y < Panel::kResY; y++)
        {
            uint8_t bgBright = 3 + (y * 5) / Panel::kResY;
            panel->drawFastHLine(0, y, Panel::kResX, panel->color565(bgBright, bgBright, bgBright + 2));
        }
    }

    if (!dhtSensor.isReady())
    {
        panel->setTextSize(1);
        panel->setTextWrap(false);
        panel->setTextColor(panel->color565(200, 200, 200));
        panel->setCursor(5, 12);
        panel->print("Loading...");
        return;
    }

    float temp = dhtSensor.getTemperature();
    float humi = dhtSensor.getHumidity();

    // Smooth the bar targets for fluid animation
    float targetBarT = temp / 50.0f;
    float targetBarH = humi / 100.0f;
    if (targetBarT > 1.0f) targetBarT = 1.0f;
    if (targetBarH > 1.0f) targetBarH = 1.0f;
    
    sensorBarSmooth_t_ += (targetBarT - sensorBarSmooth_t_) * dt * 3.0f;
    sensorBarSmooth_h_ += (targetBarH - sensorBarSmooth_h_) * dt * 3.0f;

    panel->setTextSize(1);
    panel->setTextWrap(false);

    // ════════════════════════════════════════════════════════════
    // TOP HALF: Temperature (rows 0–13)
    // ════════════════════════════════════════════════════════════

    // Icon: tiny thermometer
    panel->drawPixel(2, 2, panel->color565(220, 60, 30));
    panel->drawPixel(2, 3, panel->color565(220, 40, 20));
    panel->drawPixel(2, 4, panel->color565(200, 30, 10));

    // Temperature value
    char tempStr[8];
    int tempInt = (int)temp;
    int tempDec = ((int)(temp * 10)) % 10;
    sprintf(tempStr, "%d.%d", tempInt, tempDec);

    // Dynamic color: cool blue → warm amber
    uint8_t tR, tG, tB;
    if (temp < 20.0f)
    {
        tR = 50; tG = 150; tB = 255;
    }
    else if (temp < 30.0f)
    {
        float f = (temp - 20.0f) / 10.0f;
        tR = 50 + (uint8_t)(f * 205);
        tG = 150 + (uint8_t)(f * 105);
        tB = 255 - (uint8_t)(f * 200);
    }
    else
    {
        tR = 255; tG = 200; tB = 55;
    }

    panel->setTextColor(panel->color565(tR, tG, tB));
    panel->setCursor(5, 2);
    panel->print(tempStr);

    // Degree symbol: 2×2 pixel square (°)
    int labelX = 5 + strlen(tempStr) * 6;
    uint16_t degColor = panel->color565(tR / 2, tG / 2, tB / 2);
    panel->drawPixel(labelX + 1, 2, degColor);
    panel->drawPixel(labelX + 2, 2, degColor);
    panel->drawPixel(labelX + 1, 3, degColor);
    panel->drawPixel(labelX + 2, 3, degColor);
    // "C" label
    panel->setTextColor(degColor);
    panel->setCursor(labelX + 4, 2);
    panel->print("C");

    // Temperature progress bar (row 10–12)
    int barWidth = (int)(sensorBarSmooth_t_ * 56);
    if (barWidth < 1) barWidth = 1;

    panel->fillRect(4, 10, 56, 3, panel->color565(20, 10, 10));

    for (int x = 0; x < barWidth; x++)
    {
        float ratio = (float)x / 56.0f;
        uint8_t bR = 30 + (uint8_t)(ratio * 225);
        uint8_t bG = 100 - (uint8_t)(ratio * 70);
        uint8_t bB = 200 - (uint8_t)(ratio * 190);
        panel->drawFastVLine(4 + x, 10, 3, panel->color565(bR, bG, bB));
    }

    // ════════════════════════════════════════════════════════════
    // BOTTOM HALF: Humidity (rows 17–31)
    // ════════════════════════════════════════════════════════════

    // Icon: water drop
    panel->drawPixel(2, 18, panel->color565(30, 100, 180));
    panel->drawPixel(2, 19, panel->color565(20, 80, 160));
    panel->drawPixel(2, 20, panel->color565(40, 120, 180));

    // Humidity value
    char humiStr[8];
    int humiInt = (int)humi;
    int humiDec = ((int)(humi * 10)) % 10;
    sprintf(humiStr, "%d.%d", humiInt, humiDec);

    // Dynamic color: dry yellow → wet cyan
    uint8_t hR, hG, hB;
    if (humi < 40.0f)
    {
        hR = 200; hG = 180; hB = 50;
    }
    else if (humi < 70.0f)
    {
        float f = (humi - 40.0f) / 30.0f;
        hR = 200 - (uint8_t)(f * 170);
        hG = 180 + (uint8_t)(f * 75);
        hB = 50 + (uint8_t)(f * 205);
    }
    else
    {
        hR = 30; hG = 255; hB = 255;
    }

    panel->setTextColor(panel->color565(hR, hG, hB));
    panel->setCursor(5, 18);
    panel->print(humiStr);

    // "%" label
    int hLabelX = 5 + strlen(humiStr) * 6;
    panel->setTextColor(panel->color565(hR / 2, hG / 2, hB / 2));
    panel->setCursor(hLabelX + 1, 18);
    panel->print("%");

    // Humidity progress bar (row 26–28)
    int hBarWidth = (int)(sensorBarSmooth_h_ * 56);
    if (hBarWidth < 1) hBarWidth = 1;

    panel->fillRect(4, 26, 56, 3, panel->color565(10, 10, 20));

    for (int x = 0; x < hBarWidth; x++)
    {
        float ratio = (float)x / 56.0f;
        uint8_t bR = 20 + (uint8_t)(ratio * 10);
        uint8_t bG = 80 + (uint8_t)(ratio * 175);
        uint8_t bB = 120 + (uint8_t)(ratio * 135);
        panel->drawFastVLine(4 + x, 26, 3, panel->color565(bR, bG, bB));
    }
}
