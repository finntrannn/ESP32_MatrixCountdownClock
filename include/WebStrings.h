/**
 * @file WebStrings.h
 * @brief All user-facing Web UI text strings in one place for easy editing /
 * i18n.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

namespace WebUI {

// ─── Page & Sidebar ──────────────────────────────────────────────
#define kPageTitle "Bảng Điều Khiển Đồng Hồ"
#define kSidebarTitle "Bảng Điều Khiển"
#define kNavDisplay "Màn hình & hiển thị"
#define kNavGeneral "Cấu hình chung"
#define kNavCountdown "MH Đếm ngược"
#define kNavSensor "MH Nhiệt độ & độ ẩm"
#define kNavDateTime "MH Ngày & giờ"
#define kNavText "MH Văn bản tự do"
#define kNavWifi "Cài đặt WiFi"
#define kNavAutomation "Tự động & lịch"
#define kNavOta "Cập nhật phần mềm"

// ─── Top Action Buttons ──────────────────────────────────────────
#define kBtnSave "Lưu"
#define kBtnTestSplash "Test Splash"
#define kBtnRestart "Khởi Động Lại"
#define kConfirmRestart "Khởi động lại ESP32?"

// ─── System & WiFi Tab ───────────────────────────────────────────
#define kWifiStaTitle "Cấu hình WiFi (WiFi mà bảng LED này sẽ kết nối)"
#define kWifiStaSsid "Tên WiFi:"
#define kWifiStaPass "Mật khẩu:"
#define kWifiApTitle "Cấu hình điểm truy cập WiFi (WiFi mà bảng LED này sẽ phát ra)"
#define kWifiApSsid "Tên WiFi:"
#define kWifiApPass "Mật khẩu:"

// ─── General Display Tab ─────────────────────────────────────────
#define kGenTitle "Cài đặt chung"
#define kGenLedOn "Bật màn hình LED"
#define kGenSplashOn "Hiển thị splash screen lúc khởi động"
#define kGenSplashText "Nội dung Splash Screen (tối đa ~10 ký tự):"
#define kGenWifiIcon "Hiển thị biểu tượng WiFi"
#define kGenBrightness "Độ sáng màn hình (1 - 255):"
#define kGenScreenTitle "Chế độ màn hình hiện tại"
#define kGenScreenNote "Chỉ có tác dụng khi \"Tự động chuyển màn hình\" bị TẮT."
#define kScrCountdown "Đếm ngược"
#define kScrSensor "Đo nhiệt độ & độ ẩm"
#define kScrDateTime "Ngày & giờ"
#define kScrText "Văn bản tự do"
#define kScrSpinner "Quay số"

// ─── Countdown Tab ───────────────────────────────────────────────
#define kCdTitle "Cấu hình màn hình đếm ngược"
#define kCdShowSec "Hiển thị số giây"
#define kCdShowMsg "Hiển thị lời nhắn"
#define kCdRainbow "Bật hiệu ứng viền cầu vồng"
#define kCdTarget "Ngày/giờ mục tiêu:"
#define kCdMessage "Lời nhắn:"
#define kCdTextSpeed "Tốc độ chữ chạy (1 - 10):"
#define kCdRainbowSpeed "Tốc độ viền cầu vồng (1 - 10):"
#define kCdColorTitle "Màu sắc"
#define kCdDaysColor "Màu chữ \"CÒN... NGÀY...\":"
#define kCdTimeColor "Màu chữ giờ phút giây:"

// ─── Sensor Tab ──────────────────────────────────────────────────
#define kSenTitle "Cấu hình màn hình nhiệt độ & độ ẩm"
#define kSenBg "Bật nền phía sau của độ ẩm"
#define kSenWeatherTitle "Thời Tiết Thực Tế (Mạng)"
#define kSenOpenMeteo "Bật thời tiết thực tế (Open-Meteo)"
#define kSenWeatherNote "Cần 15p để đồng bộ dữ liệu mỗi lần bật máy."
#define kSenLat "Vĩ độ (Latitude):"
#define kSenLon "Kinh độ (Longitude):"

// ─── DateTime Tab ────────────────────────────────────────────────
#define kDtTitle "Cấu hình màn hình ngày & giờ"
#define kDtShowYear "Hiển thị số năm"
#define kDtShortYear "Rút gọn năm (24 thay vì 2024)"
#define kDtShowSec "Hiển thị số giây"
#define kDtSeparator "Kiểu phân cách ngày:"
#define kDtSepSlash "Dấu chéo (20/11)"
#define kDtSepDot "Dấu chấm (20.11)"
#define kDtSepDash "Dấu gạch (20-11)"

// ─── Text Panel Tab ──────────────────────────────────────────────
#define kTxtTitle "Cấu hình màn hình văn bản tự do"
#define kTxtScroll "Cuộn chữ"
#define kTxtContent "Nội dung hiển thị:"
#define kTxtSpeed "Tốc độ cuộn (1 - 10):"
#define kTxtSize "Kích thước chữ (1 - 4):"
#define kTxtColor "Màu chữ:"

// ─── Auto-Cycle Tab ──────────────────────────────────────────────
#define kAutoTitle "Kịch bản màn hình"
#define kAutoEnable "Bật tự động chuyển màn hình?"
#define kAutoNote \
	"0: Đếm ngược<br>1: Nhiệt độ & độ ẩm<br>2: Ngày & giờ<br>3: Văn bản tự " \
	"do.<br>4: Quay số.<br>VD: \"0,2,0\" sẽ lặp Đếm ngược -> Ngày & giờ -> Đếm ngược."
#define kAutoSeqLabel "Chuỗi trình tự lặp (phân tách qua dấu phẩy):"
#define kAutoTimersTitle "Thời gian chốt lại (giây/màn hình)"
#define kTimerSensor "Cảm biến"
#define kTimerDateTime "Ngày giờ"
#define kTimerText "Văn bản"
#define kTimerSpinner "Quay Lô tô"
#define kFwTitle "Lịch hiển thị pháo hoa"
#define kFwNote "Cách nhau bởi \";\". VD: 10:35,5;08:40,10"
#define kFwLabel "Chuỗi lịch pháo hoa:"
#define kFwTestBtn "Bắn thử pháo hoa"

// ─── OTA Tab ─────────────────────────────────────────────────────
#define kOtaTitle "Cập nhật phần mềm (OTA)"
#define kOtaNote \
	"Tải file firmware (.bin) từ PlatformIO để nâng cấp qua WiFi. Cài đặt sẽ " \
	"được giữ nguyên. Bảng LED sẽ tự khởi động lại."
#define kOtaBtn "Bắt đầu cập nhật"
#define kOtaUploading "Đang tải lên: "
#define kOtaSuccess "Hoàn tất! Đang khởi động lại..."
#define kOtaError "Lỗi mất kết nối!"

// ─── Spinner Tab ─────────────────────────────────────────────────
#define kSpinTitle "Cấu hình màn hình quay số"
#define kSpinRange "Phạm vi số:"
#define kSpinMin "Từ:"
#define kSpinMax "Đến:"
#define kSpinDur "Thời gian quay (giây):"
#define kSpinManualTitle "Điều khiển thủ công"
#define kSpinMin2 "Min (Bắt đầu):"
#define kSpinMax2 "Max (Kết thúc):"
#define kSpinDur2 "Thời lượng chạy số trớn (giây):"
#define kSpinBtn "Quay số"
#define kSpinReset "Chơi lại (Reset)"
#define kNavSpinner "MH Quay số"

#define kRestartMsg "Đang khởi động lại..."
#define kRestartNote "Vui lòng đợi vài giây và kết nối lại WiFi."
#define kSplashMsg "Đang trình diễn Splash Screen..."
#define kSplashNote "Tự động quay lại sau 10 giây."

}  // namespace WebUI
