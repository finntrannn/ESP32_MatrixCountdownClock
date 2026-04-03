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
static constexpr const char *kPageTitle = "Bảng Điều Khiển Đồng Hồ";
static constexpr const char *kSidebarTitle	= "Bảng Điều Khiển";
static constexpr const char *kNavDisplay	= "Màn hình & hiển thị";
static constexpr const char *kNavGeneral	= "Cấu hình chung";
static constexpr const char *kNavCountdown	= "MH Đếm ngược";
static constexpr const char *kNavSensor		= "MH Nhiệt độ & độ ẩm";
static constexpr const char *kNavDateTime	= "MH Ngày & giờ";
static constexpr const char *kNavText		= "MH Văn bản tự do";
static constexpr const char *kNavWifi		= "Cài đặt WiFi";
static constexpr const char *kNavAutomation = "Tự động & lịch";
static constexpr const char *kNavOta		= "Cập nhật phần mềm";

// ─── Top Action Buttons ──────────────────────────────────────────
static constexpr const char *kBtnSave		 = "Lưu";
static constexpr const char *kBtnTestSplash	 = "Test Splash";
static constexpr const char *kBtnRestart	 = "Khởi Động Lại";
static constexpr const char *kConfirmRestart = "Khởi động lại ESP32?";

// ─── System & WiFi Tab ───────────────────────────────────────────
static constexpr const char *kWifiStaTitle =
	"Cấu hình WiFi (WiFi mà bảng LED này sẽ kết nối)";
static constexpr const char *kWifiStaSsid = "Tên WiFi:";
static constexpr const char *kWifiStaPass = "Mật khẩu:";
static constexpr const char *kWifiApTitle =
	"Cấu hình điểm truy cập WiFi (WiFi mà bảng LED này sẽ phát ra)";
static constexpr const char *kWifiApSsid = "Tên WiFi:";
static constexpr const char *kWifiApPass = "Mật khẩu:";

// ─── General Display Tab ─────────────────────────────────────────
static constexpr const char *kGenTitle = "Cài đặt chung";
static constexpr const char *kGenLedOn = "Bật màn hình LED";
static constexpr const char *kGenSplashOn =
	"Hiển thị splash screen lúc khởi động";
static constexpr const char *kGenSplashText =
	"Nội dung Splash Screen (tối đa ~10 ký tự):";
static constexpr const char *kGenWifiIcon = "Hiển thị biểu tượng WiFi";
static constexpr const char *kGenBrightness = "Độ sáng màn hình (1 - 255):";
static constexpr const char *kGenScreenTitle = "Chế độ màn hình hiện tại";
static constexpr const char *kGenScreenNote =
	"Chỉ có tác dụng khi \"Tự động chuyển màn hình\" bị TẮT.";
static constexpr const char *kScrCountdown = "Đếm ngược";
static constexpr const char *kScrSensor	   = "Đo nhiệt độ & độ ẩm";
static constexpr const char *kScrDateTime  = "Ngày & giờ";
static constexpr const char *kScrText	   = "Văn bản tự do";
static constexpr const char *kScrSpinner   = "Quay số";

// ─── Countdown Tab ───────────────────────────────────────────────
static constexpr const char *kCdTitle = "Cấu hình màn hình đếm ngược";
static constexpr const char *kCdShowSec = "Hiển thị số giây";
static constexpr const char *kCdShowMsg = "Hiển thị lời nhắn";
static constexpr const char *kCdRainbow = "Bật hiệu ứng viền cầu vồng";
static constexpr const char *kCdTarget	= "Ngày/giờ mục tiêu:";
static constexpr const char *kCdMessage = "Lời nhắn:";
static constexpr const char *kCdTextSpeed = "Tốc độ chữ chạy (1 - 10):";
static constexpr const char *kCdRainbowSpeed = "Tốc độ viền cầu vồng (1 - 10):";
static constexpr const char *kCdColorTitle = "Màu sắc";
static constexpr const char *kCdDaysColor  = "Màu chữ \"CÒN... NGÀY...\":";
static constexpr const char *kCdTimeColor  = "Màu chữ giờ phút giây:";

// ─── Sensor Tab ──────────────────────────────────────────────────
static constexpr const char *kSenTitle = "Cấu hình màn hình nhiệt độ & độ ẩm";
static constexpr const char *kSenBg = "Bật nền phía sau của độ ẩm";

// ─── DateTime Tab ────────────────────────────────────────────────
static constexpr const char *kDtTitle = "Cấu hình màn hình ngày & giờ";
static constexpr const char *kDtShowYear = "Hiển thị số năm";
static constexpr const char *kDtShortYear = "Rút gọn năm (24 thay vì 2024)";
static constexpr const char *kDtShowSec	  = "Hiển thị số giây";
static constexpr const char *kDtSeparator = "Kiểu phân cách ngày:";
static constexpr const char *kDtSepSlash  = "Dấu chéo (20/11)";
static constexpr const char *kDtSepDot	  = "Dấu chấm (20.11)";
static constexpr const char *kDtSepDash	  = "Dấu gạch (20-11)";

// ─── Text Panel Tab ──────────────────────────────────────────────
static constexpr const char *kTxtTitle = "Cấu hình màn hình văn bản tự do";
static constexpr const char *kTxtScroll	 = "Cuộn chữ";
static constexpr const char *kTxtContent = "Nội dung hiển thị:";
static constexpr const char *kTxtSpeed	 = "Tốc độ cuộn (1 - 10):";
static constexpr const char *kTxtColor	 = "Màu chữ:";

// ─── Auto-Cycle Tab ──────────────────────────────────────────────
static constexpr const char *kAutoTitle = "Kịch bản màn hình";
static constexpr const char *kAutoEnable = "Bật tự động chuyển màn hình?";
static constexpr const char *kAutoNote =
	"0: Đếm ngược<br>1: Nhiệt độ & độ ẩm<br>2: Ngày & giờ<br>3: Văn bản tự "
	"do.<br>4: Quay số.<br>VD: \"0,2,0\" sẽ lặp Đếm ngược -> Ngày & giờ -> Đếm ngược.";
static constexpr const char *kAutoSeqLabel = "Thứ tự hiển thị:";
static constexpr const char *kAutoTimersTitle = "Thời gian trình chiếu (giây)";
static constexpr const char *kFwTitle = "Lịch hiển thị pháo hoa";
static constexpr const char *kFwNote =
	"Cách nhau bởi \";\". VD: 10:35,5;08:40,10";
static constexpr const char *kFwLabel	= "Chuỗi lịch pháo hoa:";
static constexpr const char *kFwTestBtn = "Bắn thử pháo hoa";

// ─── OTA Tab ─────────────────────────────────────────────────────
static constexpr const char *kOtaTitle = "Cập nhật phần mềm (OTA)";
static constexpr const char *kOtaNote =
	"Tải file firmware (.bin) từ PlatformIO để nâng cấp qua WiFi. Cài đặt sẽ "
	"được giữ nguyên. Bảng LED sẽ tự khởi động lại.";
static constexpr const char *kOtaBtn	   = "Bắt đầu cập nhật";
static constexpr const char *kOtaUploading = "Đang tải lên: ";
static constexpr const char *kOtaSuccess = "Hoàn tất! Đang khởi động lại...";
static constexpr const char *kOtaError = "Lỗi mất kết nối!";

// ─── Spinner Tab ─────────────────────────────────────────────────
static constexpr const char *kSpinTitle	  = "Cấu hình màn hình quay số";
static constexpr const char *kSpinRange	  = "Phạm vi số:";
static constexpr const char *kSpinMin	  = "Từ:";
static constexpr const char *kSpinMax	  = "Đến:";
static constexpr const char *kSpinDur	  = "Thời gian quay (giây):";
static constexpr const char *kSpinBtn	  = "Quay số";
static constexpr const char *kSpinReset	  = "Chơi lại (Reset)";
static constexpr const char *kNavSpinner  = "MH Quay số";

// ─── Restart / Splash pages ──────────────────────────────────────
static constexpr const char *kRestartMsg = "Đang khởi động lại...";
static constexpr const char *kRestartNote =
	"Vui lòng đợi vài giây và kết nối lại WiFi.";
static constexpr const char *kSplashMsg = "Đang trình diễn Splash Screen...";
static constexpr const char *kSplashNote = "Tự động quay lại sau 10 giây.";

}  // namespace WebUI
