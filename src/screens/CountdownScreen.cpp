/**
 * @file CountdownScreen.cpp
 * @brief Renders the THPT QG Countdown interface.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#include "screens/CountdownScreen.h"

#include "Config.h"

void CountdownScreen::draw(float dt, DisplayManager &display,
						   TimeManager &timeManager, const AppState &appState) {
	auto *panel = display.getPanel();

	bool showMsg = appState.isShowMessageEnabled();

	// ── 2. Current Time (top-center mini clock) ──────────────────
	time_t now = timeManager.getCurrentTime();
	if (timeManager.isTimeSynced()) {
		struct tm timeinfo_buf;
		struct tm *timeinfo = localtime_r(&now, &timeinfo_buf);
		char timeStr[6];
		sprintf(timeStr, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
		if (showMsg) {
			display.drawMiniString(23, 1, timeStr,
								   panel->color565(200, 200, 200));
		} else {
			panel->setTextSize(1);
			panel->setTextColor(panel->color565(200, 200, 200));
			panel->setCursor(18, 2);
			panel->print(timeStr);
		}
	} else {
		if (showMsg) {
			display.drawMiniString(23, 1, "--:--", panel->color565(255, 0, 0));
		} else {
			panel->setTextSize(1);
			panel->setTextColor(panel->color565(255, 0, 0));
			panel->setCursor(18, 2);
			panel->print("--:--");
		}
	}

	// ── 3. Countdown (days + HH:MM:SS) ──────────────────────────
	long diff = appState.getTargetEpoch() - now;
	if (diff < 0 || !timeManager.isTimeSynced()) diff = 0;

	long days  = diff / 86400;
	long hours = (diff % 86400) / 3600;
	long mins  = (diff % 3600) / 60;
	long secs  = diff % 60;

	panel->setTextSize(1);
	panel->setTextWrap(false);

	int daysY = showMsg ? 8 : 13;
	int hmsY  = showMsg ? 16 : 23;

	// Days row
	uint16_t daysColor =
		DisplayManager::getStandardColor(appState.getCdDaysColor());
	panel->setTextColor(daysColor);
	char numStr[16];
	sprintf(numStr, "%d", (int)days);
	int numLen = strlen(numStr);

	int spacePixels = 1;
	if (numLen == 2) spacePixels = 3;
	if (numLen == 1) spacePixels = 4;

	int totalW = 18 + spacePixels + numLen * 6 + spacePixels + 24;
	int startX = (64 - totalW) / 2;
	if (startX < 0) startX = 0;

	panel->setCursor(startX, daysY);
	panel->print("CON");
	// Accent mark for "Ò"
	panel->drawLine(startX + 7, daysY - 3, startX + 8, daysY - 2, daysColor);

	int numX = startX + 18 + spacePixels;
	panel->setCursor(numX, daysY);
	panel->print(numStr);

	int nX = numX + numLen * 6 + spacePixels;
	panel->setCursor(nX, daysY);
	panel->print("NGAY");
	// Accent mark for "À"
	panel->drawLine(nX + 13, daysY - 3, nX + 14, daysY - 2, daysColor);

	// Hours:Minutes:Seconds row
	char hStr[8], mStr[8], sStr[8];
	sprintf(hStr, "%02dh", (int)hours);
	sprintf(mStr, "%02dm", (int)mins);
	sprintf(sStr, "%02ds", (int)secs);

	int spacing	   = 2;	 // 2 pixels spacing instead of 6px (full space char)
	int totalTimeW = 18 + spacing + 18;	 // 00h and 00m
	if (appState.isShowSecondsCountdownEnabled()) {
		totalTimeW += spacing + 18;	 // array seconds
	}

	int startXTime = (64 - totalTimeW) / 2;
	if (startXTime < 0) startXTime = 0;

	uint16_t timeColor =
		DisplayManager::getStandardColor(appState.getCdTimeColor());
	panel->setTextColor(timeColor);

	panel->setCursor(startXTime, hmsY);
	panel->print(hStr);

	startXTime += 18 + spacing;
	panel->setCursor(startXTime, hmsY);
	panel->print(mStr);

	if (appState.isShowSecondsCountdownEnabled()) {
		startXTime += 18 + spacing;
		panel->setCursor(startXTime, hmsY);
		panel->print(sStr);
	}

	// ── 4. Custom Scrolling Text ─────────────────────────────────
	if (showMsg) {
		panel->setTextColor(panel->color565(50, 255, 50));
		const String &customText = appState.getCustomText();
		int textWidth			 = customText.length() * 6;

		if (textWidth <= Panel::kResX) {
			int centerX = (Panel::kResX - textWidth) / 2;
			panel->setCursor(centerX, 24);
			panel->print(customText);
		} else {
			panel->setCursor((int)textX_, 24);
			panel->print(customText);

			static uint8_t c_frame_cnt = 0;
			c_frame_cnt++;
			int fPS = 1, pPS = 1;
			switch (appState.getTextSpeed()) {
				case 1:
					fPS = 6;
					break;
				case 2:
					fPS = 5;
					break;
				case 3:
					fPS = 4;
					break;
				case 4:
					fPS = 3;
					break;
				case 5:
					fPS = 2;
					break;
				case 6:
					fPS = 1;
					pPS = 1;
					break;
				case 7:
					fPS = 1;
					pPS = 2;
					break;
				case 8:
					fPS = 1;
					pPS = 3;
					break;
				case 9:
					fPS = 1;
					pPS = 4;
					break;
				default:
					fPS = 1;
					pPS = 5;
					break;
			}
			if (c_frame_cnt >= fPS) {
				textX_ -= pPS;
				c_frame_cnt = 0;
			}

			if (textX_ < -textWidth) {
				textX_ = static_cast<float>(Panel::kResX);
			}
		}
	}

	// ── 5. Rainbow Border ────────────────────────────────────────
	if (appState.isRainbowEnabled()) {
		display.drawRainbowBorder(static_cast<uint16_t>(borderFloatOffset_));

		borderFloatOffset_ += appState.getRainbowSpeed() * 100.0f * dt;
		if (borderFloatOffset_ >= 256.0f) {
			borderFloatOffset_ -= 256.0f;
		}
	}
}
