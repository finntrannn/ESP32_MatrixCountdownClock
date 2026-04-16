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



// ─── Helper: format and draw a temperature value ─────────────────
static void drawTemp(MatrixPanel_I2S_DMA* panel,
					 int cx, int cy, float temp,
					 uint8_t r, uint8_t g, uint8_t b, bool center = false) {
	char buf[8];
	int intPart = (int)temp;
	int fracPart = abs((int)(temp * 10)) % 10;
	if (temp < 0 && intPart == 0) sprintf(buf, "-0.%d", fracPart);
	else sprintf(buf, "%d.%d", intPart, fracPart);
	int x0 = cx;
	if (center) {
		int w = strlen(buf) * 6 + 2;
		x0 = cx - w / 2;
	}
	panel->setTextColor(panel->color565(r, g, b));
	panel->setCursor(x0, cy);
	panel->print(buf);
	// Tiny ° symbol
	int dx = x0 + strlen(buf) * 6;
	uint16_t dim = panel->color565(r / 2, g / 2, b / 2);
	panel->drawPixel(dx, cy, dim);
	panel->drawPixel(dx + 1, cy, dim);
	panel->drawPixel(dx, cy + 1, dim);
	panel->drawPixel(dx + 1, cy + 1, dim);
}

// ─── Helper: format and draw a humidity value ────────────────────
static void drawHumi(MatrixPanel_I2S_DMA* panel,
					 int cx, int cy, float humi,
					 uint8_t r, uint8_t g, uint8_t b, bool center = false) {
	char buf[8];
	sprintf(buf, "%d%%", (int)humi);
	int x0 = cx;
	if (center) {
		int w = strlen(buf) * 6 - 1;
		x0 = cx - w / 2;
	}
	panel->setTextColor(panel->color565(r, g, b));
	panel->setCursor(x0, cy);
	panel->print(buf);
}

// ─── Color mapper: temperature → RGB ─────────────────────────────
static void tempToColor(float t, uint8_t& r, uint8_t& g, uint8_t& b) {
	if (t < 20.0f) {
		r = 50; g = 150; b = 255;
	} else if (t < 30.0f) {
		float f = (t - 20.0f) / 10.0f;
		r = 50 + (uint8_t)(f * 205);
		g = 150 + (uint8_t)(f * 105);
		b = 255 - (uint8_t)(f * 200);
	} else {
		r = 255; g = 200; b = 55;
	}
}

// ─── Color mapper: humidity → RGB ────────────────────────────────
static void humiToColor(float h, uint8_t& r, uint8_t& g, uint8_t& b) {
	if (h < 40.0f) {
		r = 200; g = 180; b = 50;
	} else if (h < 70.0f) {
		float f = (h - 40.0f) / 30.0f;
		r = 200 - (uint8_t)(f * 170);
		g = 180 + (uint8_t)(f * 75);
		b = 50 + (uint8_t)(f * 205);
	} else {
		r = 30; g = 255; b = 255;
	}
}

// ─── Helper: Draw Value for Grid Layout ────────────────────────
static void drawGridValue(MatrixPanel_I2S_DMA* panel,
						  int cx, int cy, float val, int type,
						  uint8_t r, uint8_t g, uint8_t b) {
	char buf[8];
	if (type == 0) {
		int intPart = (int)val;
		int fracPart = abs((int)(val * 10)) % 10;
		if (val < 0 && intPart == 0) {
			sprintf(buf, "-0.%d", fracPart);
		} else {
			sprintf(buf, "%d.%d", intPart, fracPart);
		}
	} else {
		sprintf(buf, "%d%%", (int)val);
	}
	
	int textW = strlen(buf) * 6 - 1;
	if (type == 0) textW += 2; // ° symbol
	
	int x0 = cx - textW / 2;
	uint16_t color = panel->color565(r, g, b);
	panel->setTextColor(color);
	panel->setCursor(x0, cy);
	panel->print(buf);

	if (type == 0) {
		int dx = x0 + strlen(buf) * 6;
		panel->drawPixel(dx, cy, color);
		panel->drawPixel(dx + 1, cy, color);
		panel->drawPixel(dx, cy + 1, color);
		panel->drawPixel(dx + 1, cy + 1, color);
	}
}

// ─── Helper: Draw House Icon 9x9 ──────────────────────────────
static void drawHouseIcon(MatrixPanel_I2S_DMA* panel, int x, int y, uint16_t color) {
	panel->drawPixel(x+4, y, color);
	panel->drawPixel(x+3, y+1, color); panel->drawPixel(x+5, y+1, color);
	panel->drawPixel(x+2, y+2, color); panel->drawPixel(x+6, y+2, color);
	panel->drawPixel(x+1, y+3, color); panel->drawPixel(x+7, y+3, color);
	panel->drawFastHLine(x, y+4, 9, color);
	panel->drawFastVLine(x+1, y+5, 4, color);
	panel->drawFastVLine(x+7, y+5, 4, color);
	panel->fillRect(x+3, y+6, 3, 3, color); // Door
}

// ─── Helper: Draw Sun Icon 9x9 ────────────────────────────────
static void drawSunIcon(MatrixPanel_I2S_DMA* panel, int x, int y, uint16_t color) {
	panel->fillRect(x+3, y+3, 3, 3, color); // Core
	panel->drawFastVLine(x+4, y, 2, color); // Top ray
	panel->drawFastVLine(x+4, y+7, 2, color); // Bottom ray
	panel->drawFastHLine(x, y+4, 2, color); // Left ray
	panel->drawFastHLine(x+7, y+4, 2, color); // Right ray
	panel->drawPixel(x+1, y+1, color); panel->drawPixel(x+2, y+2, color);
	panel->drawPixel(x+7, y+1, color); panel->drawPixel(x+6, y+2, color);
	panel->drawPixel(x+1, y+7, color); panel->drawPixel(x+2, y+6, color);
	panel->drawPixel(x+7, y+7, color); panel->drawPixel(x+6, y+6, color);
}

void SensorScreen::draw(float dt, DisplayManager& display,
						const DHTSensor& dhtSensor, const AppState& appState) {
	auto* panel = display.getPanel();

	// ── Background gradient (toggleable) ─────────────────────────
	if (appState.isSensorBgEnabled()) {
		for (int y = 0; y < Panel::kResY; y++) {
			uint8_t bgBright = 3 + (y * 5) / Panel::kResY;
			panel->drawFastHLine(
				0, y, Panel::kResX,
				panel->color565(bgBright, bgBright, bgBright + 2));
		}
	}

	if (!dhtSensor.isReady()) {
		panel->setTextSize(1);
		panel->setTextWrap(false);
		panel->setTextColor(panel->color565(200, 200, 200));
		panel->setCursor(5, 12);
		panel->print("Loading...");
		return;
	}

	panel->setTextSize(1);
	panel->setTextWrap(false);

	float inTemp = dhtSensor.getTemperature();
	float inHumi = dhtSensor.getHumidity();

	bool dual = appState.isOpenMeteoEnabled();

	if (dual) {
		// ══════════════════════════════════════════════════
		//  DUAL MODE: Minimalist Grid Layout
		// ══════════════════════════════════════════════════

		// Dividers
		panel->drawFastHLine(0, 16, 64, panel->color565(20, 25, 30)); // Horizontal
		panel->drawFastVLine(41, 0, 32, panel->color565(20, 25, 30)); // Vertical

		uint8_t r, g, b;

		// ── ROW 1: IN (Y=0..14) ──
		drawHouseIcon(panel, 2, 3, panel->color565(80, 220, 120)); // Soft Green house

		// Temp (Type 0)
		tempToColor(inTemp, r, g, b);
		drawGridValue(panel, 26, 4, inTemp, 0, r, g, b);

		// Humi (Type 1)
		humiToColor(inHumi, r, g, b);
		drawGridValue(panel, 52, 4, inHumi, 1, r, g, b);

		// ── ROW 2: OUT (Y=17..31) ──
		drawSunIcon(panel, 2, 20, panel->color565(255, 180, 50)); // Orange sun

		if (appState.isRealDataValid()) {
			float outTemp = appState.getRealTemp();
			float outHumi = appState.getRealHumi();

			tempToColor(outTemp, r, g, b);
			drawGridValue(panel, 26, 21, outTemp, 0, r, g, b);

			humiToColor(outHumi, r, g, b);
			drawGridValue(panel, 52, 21, outHumi, 1, r, g, b);
		} else {
			// Loading dots for OUT
			int dots = (millis() / 300) % 4;
			int w = dots * 6;
			panel->setTextColor(panel->color565(80, 100, 150));
			panel->setCursor(38 - w / 2, 21); // Centered across remaining width
			for (int i = 0; i < dots; i++) panel->print(".");
		}

	} else {
		// ══════════════════════════════════════════════════
		//  SINGLE MODE: Original full-screen layout
		// ══════════════════════════════════════════════════

		float temp = inTemp;
		float humi = inHumi;
		uint8_t r, g, b;

		float targetBarT = temp / 50.0f;
		float targetBarH = humi / 100.0f;
		if (targetBarT > 1.0f) targetBarT = 1.0f;
		if (targetBarH > 1.0f) targetBarH = 1.0f;

		sensorBarSmooth_t_ += (targetBarT - sensorBarSmooth_t_) * dt * 3.0f;
		sensorBarSmooth_h_ += (targetBarH - sensorBarSmooth_h_) * dt * 3.0f;

		// Icon: tiny thermometer
		panel->drawPixel(2, 2, panel->color565(220, 60, 30));
		panel->drawPixel(2, 3, panel->color565(220, 40, 20));
		panel->drawPixel(2, 4, panel->color565(200, 30, 10));

		tempToColor(temp, r, g, b);
		drawTemp(panel, 5, 2, temp, r, g, b);

		// "C" label after degree
		char ts[8];
		int intPart = (int)temp;
		int fracPart = abs((int)(temp * 10)) % 10;
		if (temp < 0 && intPart == 0) sprintf(ts, "-0.%d", fracPart);
		else sprintf(ts, "%d.%d", intPart, fracPart);
		int labelX = 5 + strlen(ts) * 6 + 4;
		panel->setTextColor(panel->color565(r / 2, g / 2, b / 2));
		panel->setCursor(labelX, 2);
		panel->print("C");

		int barWidth = (int)(sensorBarSmooth_t_ * 56);
		if (barWidth < 1) barWidth = 1;
		panel->fillRect(4, 10, 56, 3, panel->color565(20, 10, 10));
		for (int x = 0; x < barWidth; x++) {
			float ratio = (float)x / 56.0f;
			panel->drawFastVLine(4 + x, 10, 3, panel->color565(
				30 + (uint8_t)(ratio * 225),
				100 - (uint8_t)(ratio * 70),
				200 - (uint8_t)(ratio * 190)));
		}

		// Icon: water drop
		panel->drawPixel(2, 18, panel->color565(30, 100, 180));
		panel->drawPixel(2, 19, panel->color565(20, 80, 160));
		panel->drawPixel(2, 20, panel->color565(40, 120, 180));

		humiToColor(humi, r, g, b);
		drawHumi(panel, 5, 18, humi, r, g, b);

		int hBarWidth = (int)(sensorBarSmooth_h_ * 56);
		if (hBarWidth < 1) hBarWidth = 1;
		panel->fillRect(4, 26, 56, 3, panel->color565(10, 10, 20));
		for (int x = 0; x < hBarWidth; x++) {
			float ratio = (float)x / 56.0f;
			panel->drawFastVLine(4 + x, 26, 3, panel->color565(
				20 + (uint8_t)(ratio * 10),
				80 + (uint8_t)(ratio * 175),
				120 + (uint8_t)(ratio * 135)));
		}
	}
}
