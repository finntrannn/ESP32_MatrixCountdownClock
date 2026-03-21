/**
 * @file DisplayManager.h
 * @brief LED Matrix panel abstraction and drawing utilities.
 * 
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

class DisplayManager {
public:
    /**
     * @brief Initialize the HUB75 DMA display with double buffering.
     * @param brightness Initial brightness (1–255).
     */
    void begin(int brightness);

    /** @brief Get raw panel pointer for advanced operations. */
    MatrixPanel_I2S_DMA* getPanel() { return panel_; }

    /** @brief Set display brightness (1–255). */
    void setBrightness(int brightness);

    /** @brief Clear the screen (current back-buffer). */
    void clear();

    /** @brief Flip the DMA double buffer to show the rendered frame. */
    void flip();

    // ─── Mini 3×5 Font ───────────────────────────────────────────
    /**
     * @brief Draw a single character using the 3×5 bitmap font.
     * @param x, y  Top-left pixel position.
     * @param c     Character ('0'–'9', ':', '-').
     * @param color 16-bit RGB565 color.
     */
    void drawMiniChar(int x, int y, char c, uint16_t color);

    /**
     * @brief Draw a null-terminated string using the 3×5 bitmap font.
     * @param x, y  Top-left pixel position.
     * @param str   String to render.
     * @param color 16-bit RGB565 color.
     */
    void drawMiniString(int x, int y, const char* str, uint16_t color);

    // ─── Color Utilities ─────────────────────────────────────────
    /**
     * @brief Generate a rainbow color from a position on the color wheel.
     * @param wheelPos Position (0–255) on the color wheel.
     * @return 16-bit RGB565 color.
     */
    uint16_t wheel(uint8_t wheelPos);

    /**
     * @brief Draw an animated rainbow border around the panel.
     * @param offset Animation offset for cycling colors.
     */
    void drawRainbowBorder(uint16_t offset);

private:
    MatrixPanel_I2S_DMA* panel_ = nullptr;

    /** @brief Bitmap font data for digits 0–9, colon, and hyphen (3×5). */
    static const uint16_t kDigits3x5[12];
};
