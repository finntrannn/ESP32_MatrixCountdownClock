/**
 * @file DisplayManager.cpp
 * @brief Implementation of HUB75 LED Matrix panel management.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "DisplayManager.h"

#include "Config.h"

// 3×5 bitmap font data: digits 0–9, colon (:), hyphen (-)
const uint16_t DisplayManager::kDigits3x5[12] = {
	0x7B6F,	 // 0
	0x2C97,	 // 1
	0x73E7,	 // 2
	0x73CF,	 // 3
	0x5BC9,	 // 4
	0x79CF,	 // 5
	0x79EF,	 // 6
	0x7292,	 // 7
	0x7BEF,	 // 8
	0x7BCF,	 // 9
	0x0410,	 // : (index 10)
	0x01C0	 // - (index 11)
};

void DisplayManager::begin(int brightness) {
	HUB75_I2S_CFG mxconfig(Panel::kResX, Panel::kResY, Panel::kChain);
	mxconfig.double_buff	= true;
	mxconfig.latch_blanking = 3;
	mxconfig.i2sspeed		= HUB75_I2S_CFG::HZ_10M;
	mxconfig.clkphase		= false;
	panel_					= new MatrixPanel_I2S_DMA(mxconfig);
	panel_->begin();
	panel_->setBrightness8(brightness);
	panel_->clearScreen();
}

void DisplayManager::setBrightness(int brightness) {
	if (panel_) {
		panel_->setBrightness8(brightness);
	}
}

void DisplayManager::clear() {
	if (panel_) {
		panel_->clearScreen();
	}
}

void DisplayManager::flip() {
	if (panel_) {
		panel_->flipDMABuffer();
	}
}

void IRAM_ATTR DisplayManager::drawMiniChar(int x, int y, char c,
											uint16_t color) {
	int index = -1;
	if (c >= '0' && c <= '9')
		index = c - '0';
	else if (c == ':')
		index = 10;
	else if (c == '-')
		index = 11;
	if (index < 0) return;

	uint16_t bits = kDigits3x5[index];
	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 3; col++) {
			int bitIdx = 14 - (row * 3 + col);
			if ((bits >> bitIdx) & 1) {
				panel_->drawPixel(x + col, y + row, color);
			}
		}
	}
}

void IRAM_ATTR DisplayManager::drawMiniString(int x, int y, const char *str,
											  uint16_t color) {
	int i = 0;
	while (str[i]) {
		drawMiniChar(x + i * 4, y, str[i], color);
		i++;
	}
}

uint16_t IRAM_ATTR DisplayManager::wheel(uint8_t wheelPos) {
	if (wheelPos < 85) {
		return panel_->color565(255 - wheelPos * 3, 0, wheelPos * 3);
	} else if (wheelPos < 170) {
		wheelPos -= 85;
		return panel_->color565(0, wheelPos * 3, 255 - wheelPos * 3);
	} else {
		wheelPos -= 170;
		return panel_->color565(wheelPos * 3, 255 - wheelPos * 3, 0);
	}
}

void DisplayManager::drawRainbowBorder(uint16_t offset) {
	// Top edge (left to right)
	for (int x = 0; x < Panel::kResX; x++) {
		panel_->drawPixel(x, 0, wheel(((x * 256 / 188) + offset) & 255));
	}
	// Right edge (top to bottom)
	for (int y = 1; y < Panel::kResY - 1; y++) {
		int idx = 64 + (y - 1);
		panel_->drawPixel(Panel::kResX - 1, y,
						  wheel(((idx * 256 / 188) + offset) & 255));
	}
	// Bottom edge (right to left)
	for (int x = Panel::kResX - 1; x >= 0; x--) {
		int idx = 94 + (63 - x);
		panel_->drawPixel(x, Panel::kResY - 1,
						  wheel(((idx * 256 / 188) + offset) & 255));
	}
	// Left edge (bottom to top)
	for (int y = Panel::kResY - 2; y >= 1; y--) {
		int idx = 158 + (30 - y);
		panel_->drawPixel(0, y, wheel(((idx * 256 / 188) + offset) & 255));
	}
}

uint16_t DisplayManager::getStandardColor(int index) {
	// Uses MatrixPanel color565 bit-packing: (r & 0xF8) << 8 | (g & 0xFC) << 3
	// | b >> 3
	switch (index) {
		case 0:
			return 0x07E0;	// Green
		case 1:
			return 0xF800;	// Red
		case 2:
			return 0x001F;	// Blue
		case 3:
			return 0xFFE0;	// Yellow
		case 4:
			return 0x07FF;	// Cyan
		case 5:
			return 0xF81F;	// Magenta
		default:
			return 0xFFFF;	// White
	}
}
