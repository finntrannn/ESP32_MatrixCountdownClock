/**
 * @file RandomSpinnerScreen.cpp
 * @brief Random number spinner with rapid cycling animation and win flash.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/RandomSpinnerScreen.h"

#include <cstdlib>

#include "Config.h"

void RandomSpinnerScreen::resetAnimations() {
	state_			  = IDLE;
	spinTimer_		  = 0.0f;
	resultFlashTimer_ = 0.0f;
	currentDisplayNum_ = 0;
	finalResult_	   = 0;
}

void RandomSpinnerScreen::parseExcluded(const String& csv) {
	excludedCount_ = 0;
	if (csv.length() == 0) return;
	int startIdx = 0;
	while (startIdx < (int)csv.length() && excludedCount_ < 200) {
		int commaIdx = csv.indexOf(',', startIdx);
		if (commaIdx == -1) commaIdx = csv.length();
		String numStr = csv.substring(startIdx, commaIdx);
		startIdx	  = commaIdx + 1;
		if (numStr.length() > 0) {
			excludedList_[excludedCount_++] = numStr.toInt();
		}
	}
}

bool RandomSpinnerScreen::isExcluded(int num) {
	for (int i = 0; i < excludedCount_; i++) {
		if (excludedList_[i] == num) return true;
	}
	return false;
}

int RandomSpinnerScreen::pickRandom(int minVal, int maxVal) {
	if (minVal > maxVal) return minVal;

	// Count available numbers
	int available = 0;
	for (int i = minVal; i <= maxVal; i++) {
		if (!isExcluded(i)) available++;
	}
	if (available == 0) return -1;  // All excluded

	int pick = random(0, available);
	int count = 0;
	for (int i = minVal; i <= maxVal; i++) {
		if (!isExcluded(i)) {
			if (count == pick) return i;
			count++;
		}
	}
	return minVal;  // fallback
}

void RandomSpinnerScreen::drawNumber(DisplayManager& display, int num,
									 uint16_t color) {
	auto* panel = display.getPanel();
	char buf[8];
	snprintf(buf, sizeof(buf), "%d", num);
	int len		 = strlen(buf);
	int charW	 = 12;	// Size 2 font width
	int totalW	 = len * charW;
	int x		 = (Panel::kResX - totalW) / 2;
	int y		 = 9;  // Vertically centered for size 2
	panel->setTextSize(2);
	panel->setTextColor(color);
	panel->setCursor(x, y);
	panel->print(buf);
}

void RandomSpinnerScreen::drawCenteredText(DisplayManager& display,
										   const char* text, int y,
										   uint16_t color) {
	auto* panel = display.getPanel();
	int len		= strlen(text);
	int x		= (Panel::kResX - len * 6) / 2;
	if (x < 0) x = 0;
	panel->setTextSize(1);
	panel->setTextColor(color);
	panel->setCursor(x, y);
	panel->print(text);
}

void RandomSpinnerScreen::draw(float dt, DisplayManager& display,
							   AppState& appState) {
	auto* panel = display.getPanel();

	// Handle spin reset request
	if (appState.consumeSpinResetRequest()) {
		appState.setExcludedNumbers("");
		state_ = IDLE;
		finalResult_ = 0;
	}

	// (Spin-next request is removed. Results are auto-excluded at the end of the spin.)

	// Handle spin request
	if (appState.consumeSpinRequest()) {
		if (state_ != SPINNING) {
			parseExcluded(appState.getExcludedNumbers());
			int minVal = appState.getSpinRangeMin();
			int maxVal = appState.getSpinRangeMax();

			// Pre-pick final result
			finalResult_ = pickRandom(minVal, maxVal);
			if (finalResult_ == -1) {
				// All numbers excluded
				state_ = IDLE;
				finalResult_ = 0;
			} else {
				spinDuration_  = (float)appState.getSpinDuration();
				spinTimer_	   = 0.0f;
				flickerTimer_  = 0.0f;
				state_		   = SPINNING;
			}
		}
	}

	panel->setTextWrap(false);

	int minVal = appState.getSpinRangeMin();
	int maxVal = appState.getSpinRangeMax();

	switch (state_) {
		case IDLE: {
			// Show "QUAY SO" title and range info
			drawCenteredText(display, "QUAY SO", 4,
							 panel->color565(100, 200, 255));

			char rangeStr[20];
			snprintf(rangeStr, sizeof(rangeStr), "%d - %d", minVal, maxVal);
			drawCenteredText(display, rangeStr, 16,
							 panel->color565(200, 200, 200));

			// Show "San sang!" at bottom
			// drawCenteredText(display, "San sang!", 25,
			// 				 panel->color565(50, 255, 50));
			break;
		}

		case SPINNING: {
			spinTimer_ += dt;
			flickerTimer_ += dt;

			// Slow down cycling as we approach the end
			float progress = spinTimer_ / spinDuration_;
			// Đường cong cubic easing để hiệu ứng chậm dần rõ rệt và mượt mà hơn
			float cubic = progress * progress * progress;
			float interval = 0.03f + cubic * 0.5f;

			if (flickerTimer_ >= interval) {
				flickerTimer_ = 0.0f;
				
				// Nếu tick này là tick cuối cùng trước khi hết giờ, hiển thị kết quả luôn
				if (spinTimer_ + interval >= spinDuration_) {
					currentDisplayNum_ = finalResult_;
				} else {
					// Generate random display number, avoid showing same number twice
					int rNum = random(minVal, maxVal + 1);
					if (rNum == currentDisplayNum_ && maxVal > minVal) {
						rNum = minVal + (rNum - minVal + 1) % (maxVal - minVal + 1);
					}
					currentDisplayNum_ = rNum;
				}
			}

			// Draw the cycling number with color based on progress
			uint8_t r = 255;
			uint8_t g = (uint8_t)(255 * (1.0f - progress));
			uint8_t b = (uint8_t)(100 * progress);
			drawNumber(display, currentDisplayNum_, panel->color565(r, g, b));

			if (spinTimer_ >= spinDuration_) {
				state_			  = RESULT;
				resultFlashTimer_ = 0.0f;
				
				// Auto-exclude the result
				if (finalResult_ > 0) {
					String excl = appState.getExcludedNumbers();
					if (excl.length() > 0) excl += ",";
					excl += String(finalResult_);
					appState.setExcludedNumbers(excl);
				}
			}
			break;
		}

		case RESULT: {
			resultFlashTimer_ += dt;

			uint16_t numColor;

			// Tạm dừng 0.8s để người chơi nhìn rõ số trúng thưởng (giữ nguyên màu giống lúc vừa dừng quay)
			if (resultFlashTimer_ < 0.8f) {
				numColor = panel->color565(255, 0, 100);
				drawNumber(display, finalResult_, numColor);
			}
			// Bắt đầu chớp nháy ăn mừng trong 2.2 giây tiếp theo
			else if (resultFlashTimer_ < 3.0f) {
				float flash	   = (resultFlashTimer_ - 0.8f) * 6.0f;
				int flashPhase = ((int)flash) % 2;

				// Chớp màu chữ vàng đỏ
				numColor = flashPhase ? panel->color565(255, 255, 0)
									  : panel->color565(255, 100, 50);
				drawNumber(display, finalResult_, numColor);

				// Chớp đèn viền chạy quanh
				uint16_t borderClr = flashPhase
										 ? panel->color565(255, 150, 0)
										 : panel->color565(0, 200, 255);
				panel->drawFastHLine(0, 0, 64, borderClr);
				panel->drawFastHLine(0, 31, 64, borderClr);
				panel->drawFastVLine(0, 0, 32, borderClr);
				panel->drawFastVLine(63, 0, 32, borderClr);
			}
			// Trở về trạng thái tĩnh màu vàng kim sau khi nháy xong
			else {
				numColor = panel->color565(255, 215, 0);
				drawNumber(display, finalResult_, numColor);
			}
			break;
		}
	}
}
