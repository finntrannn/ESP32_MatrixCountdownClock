/**
 * @file SplashScreen.cpp
 * @brief Implementation of the 12A6 splash animation.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "SplashScreen.h"
#include "Config.h"
#include <WebServer.h>

void SplashScreen::play(DisplayManager& display, WebServer* server, bool force) {
    if (!enabled_ && !force)
        return;

    auto* panel = display.getPanel();
    unsigned long startMillis = millis();
    unsigned long t = 0;

    // Initialize star field
    Star stars[15];
    for (int i = 0; i < 15; i++) {
        stars[i].x     = random(0, 64);
        stars[i].y     = random(0, 32);
        stars[i].speed = random(1, 4);
    }

    while ((t = millis() - startMillis) < 15000) {
        // Keep web server responsive during animation
        if (!force && server) {
            server->handleClient();
        }

        panel->clearScreen();

        // -- Star field background --
        for (int i = 0; i < 15; i++) {
            stars[i].x -= stars[i].speed;
            if (stars[i].x < 0) {
                stars[i].x = 63;
                stars[i].y = random(0, 32);
            }
            uint8_t b = stars[i].speed * 60;
            panel->drawPixel(stars[i].x, stars[i].y, panel->color565(b, b, b));
        }

        int cx = 9;
        int cy = 9;

        panel->setTextSize(2);
        panel->setTextWrap(false);
        uint16_t textColor = panel->color565(0, 255, 255);

        // Phase 1: Scanning lines (0–500ms)
        if (t < 500) {
            int lineY = (t * 32) / 500;
            panel->drawLine(0, lineY, 63, lineY, panel->color565(0, 255, 0));
            panel->drawLine(0, 31 - lineY, 63, 31 - lineY, panel->color565(255, 0, 0));
        }
        // Phase 2: Glitch text (500–1500ms)
        else if (t < 1500) {
            if (random(0, 10) > 7) {
                cx += random(-3, 4);
                cy += random(-1, 2);
                textColor = panel->color565(random(100, 255), random(100, 255), random(100, 255));
            } else {
                uint8_t fade = ((t - 500) * 255) / 1000;
                textColor = panel->color565(0, fade, fade);
            }
            panel->setTextColor(textColor);
            panel->setCursor(cx, cy);
            panel->print("12A6");

            if (random(0, 10) > 6) {
                int gx = random(0, 64);
                int gy = random(8, 24);
                int gw = random(5, 20);
                panel->drawLine(gx, gy, gx + gw, gy, panel->color565(255, 255, 255));
            }
        }
        // Phase 3: Rainbow wheel text (1500–4000ms)
        else if (t < 4000) {
            uint8_t wPos = ((t - 1500) / 4) & 255;
            panel->setTextColor(display.wheel(wPos));
            panel->setCursor(cx, cy);
            panel->print("12A6");
        }
        // Phase 4: Fade out with curtain close (4000–5000ms)
        else if (t < 5000) {
            int timeLeft = 5000 - t;
            uint8_t brightness = (timeLeft * 255) / 1000;
            panel->setTextColor(panel->color565(brightness, brightness, brightness));

            panel->setCursor(cx, cy);
            panel->print("12A6");

            int wallHeight = ((t - 4000) * 16) / 1000;
            panel->fillRect(0, 0, 64, wallHeight, 0);
            panel->fillRect(0, 32 - wallHeight, 64, wallHeight, 0);
        }
        // Phase 5: Curtain open with gold text (5000–6000ms)
        else if (t < 6000) {
            int openHeight = ((t - 5000) * 16) / 1000;

            panel->setTextColor(panel->color565(255, 255, 0));
            panel->setCursor(cx, cy);
            panel->print("12A6");

            panel->fillRect(0, 0, 64, 16 - openHeight, 0);
            panel->fillRect(0, 16 + openHeight, 64, 16 - openHeight, 0);
        }
        // Phase 6: Steady gold text with stars (6000–9000ms)
        else if (t < 9000) {
            panel->setTextColor(panel->color565(255, 255, 0));
            panel->setCursor(cx, cy);
            panel->print("12A6");
        }
        // Phase 7: White flash × 6 (9000–15000ms)
        else {
            int fadeStage = (t - 9000) % 1000;

            if (fadeStage < 300) {
                uint8_t bright = (fadeStage * 255) / 300;
                panel->fillRect(0, 0, 64, 32, panel->color565(bright, bright, bright));
            } else {
                int timeFadeOut = fadeStage - 300;
                int bright = 255 - ((timeFadeOut * 255) / 700);
                if (bright < 0) bright = 0;
                panel->fillRect(0, 0, 64, 32, panel->color565(bright, bright, bright));
            }
        }

        panel->flipDMABuffer();
        delay(15);
    }

    panel->clearScreen();
    panel->flipDMABuffer();
}
