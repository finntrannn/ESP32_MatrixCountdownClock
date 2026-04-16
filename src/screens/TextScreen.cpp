/**
 * @file TextScreen.cpp
 * @brief Implementation of free-form text display with Vietnamese font.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/TextScreen.h"

extern const uint8_t u8g2_font_unifont_t_vietnamese1[];

TextScreen::TextScreen() : scrollPos_(64), scrollAccum_(0) {}

void TextScreen::resetAnimations() {
	scrollPos_   = 64;
	scrollAccum_ = 0;
}

void TextScreen::draw(float dt, DisplayManager& display,
					  const AppState& appState) {
	auto* panel = display.getPanel();
	const String& text = appState.getTextPanelContent();
	bool scrollEnabled = appState.isTextPanelScrollEnabled();

	int textPanelSize = appState.getTextPanelSize();
	if (textPanelSize < 1) textPanelSize = 1;

	// ─── 1. Rebuild Offscreen Canvas only when Text, Mode, or Size changes ───
	if (text != lastText_ || scrollEnabled != lastScrollEnabled_ || textPanelSize != lastSize_ || textCanvas_ == nullptr) {
		lastText_          = text;
		lastScrollEnabled_ = scrollEnabled;
		lastSize_          = textPanelSize;

		// Get text width before allocating canvas
		u8g2_for_adafruit_gfx.begin(*panel);
		u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese1);
		cachedTextWidth_ = u8g2_for_adafruit_gfx.getUTF8Width(text.c_str());

		// Clean up old canvas
		if (textCanvas_ != nullptr) {
			delete textCanvas_;
			textCanvas_ = nullptr;
		}

		if (scrollEnabled) {
			canvasWidth_  = (cachedTextWidth_ > 0) ? cachedTextWidth_ : 1;
			canvasHeight_ = 16;
		} else {
			canvasWidth_  = 64 / textPanelSize;
			canvasHeight_ = 32 / textPanelSize;
		}

		// Allocate 1-bit offscreen canvas
		textCanvas_ = new GFXcanvas1(canvasWidth_, canvasHeight_);
		textCanvas_->fillScreen(0);

		u8g2_for_adafruit_gfx.begin(*textCanvas_);
		u8g2_for_adafruit_gfx.setFont(u8g2_font_unifont_t_vietnamese1);
		u8g2_for_adafruit_gfx.setForegroundColor(1);
		u8g2_for_adafruit_gfx.setFontMode(0);

		if (scrollEnabled) {
			// Draw single line at canvas bounds
			u8g2_for_adafruit_gfx.setCursor(0, 14);
			u8g2_for_adafruit_gfx.print(text.c_str());
		} else {
			// Draw wrapped multi-line text fully into canvas
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
						if (x + wWidth > canvasWidth_ && x > 0) {
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

	// ─── 2. Blit (Copy Memory) from Canvas to Panel ───
	uint16_t color = DisplayManager::hexToColor565(appState.getTextPanelColor());
	int size = textPanelSize;

	int w = canvasWidth_;
	int h = canvasHeight_;
	uint8_t* buffer = textCanvas_->getBuffer();
	int byteWidth = (w + 7) / 8;

	int displayHeight = 32;
	int scaledHeight = h * size;
	int scaledWidth = cachedTextWidth_ * size;
	
	if (scrollEnabled) {
		if (scaledWidth <= 64) {
			// Center horizontally without moving
			int centerX = (64 - scaledWidth) / 2;
			int baseYOffset = (displayHeight - scaledHeight) / 2;

			for (int j = 0; j < h; j++) {
				int bufferYOffset = j * byteWidth;
				int baseY = baseYOffset + (j * size);
				for (int i = 0; i < w; i++) {
					if ((i & 7) == 0 && buffer[bufferYOffset + (i >> 3)] == 0) {
						i += 7;
						continue;
					}
					if (buffer[bufferYOffset + (i >> 3)] & (0x80 >> (i & 7))) {
						int baseX = centerX + (i * size);
						if (size == 1) {
							panel->drawPixel(baseX, baseY, color);
						} else {
							for (int dy = 0; dy < size; dy++) {
								for (int dx = 0; dx < size; dx++) {
									panel->drawPixel(baseX + dx, baseY + dy, color);
								}
							}
						}
					}
				}
			}
		} else {
			// ── Bresenham integer accumulator ──
			int speedNum, speedDen;
			switch (appState.getTextPanelSpeed()) {
				case 1:  speedNum = 1; speedDen = 4; break;
				case 2:  speedNum = 1; speedDen = 3; break;
				case 3:  speedNum = 1; speedDen = 2; break;
				case 4:  speedNum = 2; speedDen = 3; break;
				case 5:  speedNum = 3; speedDen = 4; break;
				case 6:  speedNum = 1; speedDen = 1; break;
				case 7:  speedNum = 5; speedDen = 4; break;
				case 8:  speedNum = 3; speedDen = 2; break;
				case 9:  speedNum = 7; speedDen = 4; break;
				case 10: speedNum = 2; speedDen = 1; break;
				default: speedNum = 1; speedDen = 1; break;
			}

			scrollAccum_ += speedNum;
			int step      = scrollAccum_ / speedDen;
			scrollAccum_ %= speedDen;
			scrollPos_   -= step;

			int scrollLimit = -scaledWidth;
			if (scrollPos_ < scrollLimit) {
				scrollPos_   = 64;
				scrollAccum_ = 0;
			}

			// Viewport-clipped blit with scaling
			int baseYOffset = (displayHeight - scaledHeight) / 2;
			int i_start = -scrollPos_ / size;
			if (i_start < 0) i_start = 0;
			
			int i_end = (64 - scrollPos_ + size - 1) / size;
			if (i_end > w) i_end = w;

			if (i_start < i_end) {
				for (int j = 0; j < h; j++) {
					int bufferYOffset = j * byteWidth;
					int baseY = baseYOffset + (j * size);
					for (int i = i_start; i < i_end; i++) {
						if ((i & 7) == 0 && buffer[bufferYOffset + (i >> 3)] == 0) {
							// If the remaining bytes are within bounds, skip
							if (i + 7 < i_end) {
								i += 7;
								continue;
							}
						}
						if (buffer[bufferYOffset + (i >> 3)] & (0x80 >> (i & 7))) {
							int baseX = scrollPos_ + (i * size);
							if (size == 1) {
								panel->drawPixel(baseX, baseY, color);
							} else {
								for (int dy = 0; dy < size; dy++) {
									for (int dx = 0; dx < size; dx++) {
										panel->drawPixel(baseX + dx, baseY + dy, color);
									}
								}
							}
						}
					}
				}
			}
		}
	} else {
		// Static Multi-line - draw universally scaled
		int baseYOffset = (displayHeight - scaledHeight) / 2;
		for (int j = 0; j < h; j++) {
			int bufferYOffset = j * byteWidth;
			int baseY = baseYOffset + (j * size);
			for (int i = 0; i < w; i++) {
				if ((i & 7) == 0 && buffer[bufferYOffset + (i >> 3)] == 0) {
					i += 7;
					continue;
				}
				if (buffer[bufferYOffset + (i >> 3)] & (0x80 >> (i & 7))) {
					int baseX = i * size;
					if (size == 1) {
						panel->drawPixel(baseX, baseY, color);
					} else {
						for (int dy = 0; dy < size; dy++) {
							for (int dx = 0; dx < size; dx++) {
								panel->drawPixel(baseX + dx, baseY + dy, color);
							}
						}
					}
				}
			}
		}
	}
}

