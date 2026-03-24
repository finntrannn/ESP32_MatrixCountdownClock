/**
 * @file TextScreen.cpp
 * @brief Implementation of free-form text display with Vietnamese font.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/TextScreen.h"

extern const uint8_t u8g2_font_unifont_t_vietnamese1[];

TextScreen::TextScreen() : textX_(64.0f) {}

void TextScreen::resetAnimations() {
    textX_ = 64.0f;
    scrollAccum_ = 0.0f;
}

void TextScreen::draw(float dt, DisplayManager& display, const AppState& appState) {
    auto* panel = display.getPanel();

    if (!u8g2_initialized_) {
        u8g2_for_adafruit_gfx.begin(*panel);
        u8g2_initialized_ = true;
    }

    // Set font based on size
    // Note: Due to U8g2 library structure, only unifont_t_vietnamese1 is fully populated.
    u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese1); // 16px Vietnamese
    // Set color
    uint16_t color = DisplayManager::getStandardColor(appState.getTextPanelColor());
    u8g2_for_adafruit_gfx.setForegroundColor(color);
    u8g2_for_adafruit_gfx.setFontMode(1); // Transparent background

    const String& text = appState.getTextPanelContent();
    int textWidth = u8g2_for_adafruit_gfx.getUTF8Width(text.c_str());

    int yPos = 20;

    if (appState.isTextPanelScrollEnabled()) {
        if (textWidth <= 64) {
            int centerX = (64 - textWidth) / 2;
            u8g2_for_adafruit_gfx.setCursor(centerX, yPos);
            u8g2_for_adafruit_gfx.print(text.c_str());
        } else {
            u8g2_for_adafruit_gfx.setCursor((int)textX_, yPos);
            u8g2_for_adafruit_gfx.print(text.c_str());

            scrollAccum_ += appState.getTextPanelSpeed() * 8.0f * dt;
            int step = (int)scrollAccum_;
            if (step > 0) {
                textX_ -= step;
                scrollAccum_ -= step;
            }
            if (textX_ < -textWidth) {
                textX_ = 64.0f;
                scrollAccum_ = 0.0f;
            }
        }
    } else {
        int x = 0;
        int y = 14; 
        int lineHeight = 16;
        int spaceWidth = u8g2_for_adafruit_gfx.getUTF8Width(" ");
        
        int wordStart = 0;
        int len = text.length();

        for (int i = 0; i <= len; i++) {
            if (i == len || text.charAt(i) == ' ' || text.charAt(i) == '\n') {
                if (i > wordStart) {
                    int wordLen = i - wordStart;
                    if (wordLen > 255) wordLen = 255;
                    char wordBuf[256];
                    strncpy(wordBuf, text.c_str() + wordStart, wordLen);
                    wordBuf[wordLen] = '\0';

                    int wWidth = u8g2_for_adafruit_gfx.getUTF8Width(wordBuf);
                    if (x + wWidth > 64 && x > 0) { 
                        x = 0;
                        y += lineHeight;
                    }
                    u8g2_for_adafruit_gfx.setCursor(x, y);
                    u8g2_for_adafruit_gfx.print(wordBuf);
                    
                    x += wWidth + spaceWidth;
                }
                wordStart = i + 1;
                
                if (i < len && text.charAt(i) == '\n') {
                    x = 0;
                    y += lineHeight;
                }
            }
        }
    }
}
