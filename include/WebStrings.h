/**
 * @file WebStrings.h
 * @brief All user-facing Web UI text strings in one place for easy editing / i18n.
 *
 * @author finntrannn (finntrannn.id.vn)
 * @github https://github.com/finntrannn
 */

#pragma once

namespace WebUI {

// ─── Page & Sidebar ──────────────────────────────────────────────
static const char* kPageTitle        = "Thiết Lập Đồng Hồ";
static const char* kSidebarTitle     = "Bảng Điều Khiển";
static const char* kNavDisplay       = "Màn Hình Hiển Thị";
static const char* kNavGeneral       = "Trình Chiếu & Cơ Bản";
static const char* kNavCountdown     = "Màn Đếm Ngược";
static const char* kNavSensor        = "Màn Đo Nhiệt Độ";
static const char* kNavDateTime      = "Ngày & Giờ";
static const char* kNavText          = "Màn Văn Bản Tự Do";
static const char* kNavWifi          = "Cài Đặt WiFi";
static const char* kNavAutomation    = "Tự Động & Lịch";
static const char* kNavOta           = "Cập Nhật Phần Mềm";

// ─── Top Action Buttons ──────────────────────────────────────────
static const char* kBtnSave          = "Lưu";
static const char* kBtnTestSplash    = "Test Splash";
static const char* kBtnRestart       = "Khởi Động Lại";
static const char* kConfirmRestart   = "Khởi động lại ESP32?";

// ─── System & WiFi Tab ───────────────────────────────────────────
static const char* kWifiStaTitle     = "Kết Nối Internet (Bảng LED nhận WiFi)";
static const char* kWifiStaSsid      = "Tên WiFi cần bắt:";
static const char* kWifiStaPass      = "Mật khẩu kết nối WiFi:";
static const char* kWifiApTitle      = "Kết Nối Cục Bộ (Trạm Phát WiFi)";
static const char* kWifiApSsid       = "Tên WiFi bảng LED:";
static const char* kWifiApPass       = "Mật khẩu WiFi (min 8):";

// ─── General Display Tab ─────────────────────────────────────────
static const char* kGenTitle         = "Thông Số Chung Bảng LED";
static const char* kGenLedOn        = "Bật Màn hình LED";
static const char* kGenSplashOn     = "Hiển thị Splash lúc khởi động";
static const char* kGenWifiIcon     = "Hiển thị tín hiệu WiFi (góc phải dưới)";
static const char* kGenBrightness   = "Độ sáng (1 - 255):";
static const char* kGenScreenTitle  = "Màn Hình Cố Định Hiện Tại";
static const char* kGenScreenNote   = "Chỉ có tác dụng khi Tự Động Xoay bị TẮT.";
static const char* kScrCountdown    = "Đếm Ngược";
static const char* kScrSensor       = "Đo Nhiệt Độ / Cảm Biến";
static const char* kScrDateTime     = "Ngày & Thời Gian";
static const char* kScrText         = "Văn Bản Tự Do";

// ─── Countdown Tab ───────────────────────────────────────────────
static const char* kCdTitle          = "Cấu Hình Đếm Ngược";
static const char* kCdShowSec        = "Hiển thị số Giây";
static const char* kCdShowMsg        = "Hiển thị lời nhắn chạy tự động";
static const char* kCdRainbow        = "Bật hiệu ứng viền cầu vồng";
static const char* kCdTarget         = "Ngày/Giờ Mục Tiêu:";
static const char* kCdMessage        = "Lời nhắn động lực:";
static const char* kCdTextSpeed      = "Tốc độ chữ chạy (1 - 10):";
static const char* kCdRainbowSpeed   = "Tốc độ viền cầu vồng (1 - 10):";
static const char* kCdColorTitle     = "Màu Sắc";
static const char* kCdDaysColor      = "Màu chữ \"CỦN ... NGÀY\":";
static const char* kCdTimeColor      = "Màu chữ giờ phút giây:";

// ─── Sensor Tab ──────────────────────────────────────────────────
static const char* kSenTitle         = "Cấu Hình Cảm Biến";
static const char* kSenBg            = "Bật nền Gradient sương mờ";

// ─── DateTime Tab ────────────────────────────────────────────────
static const char* kDtTitle          = "Cấu Hình Format Ngày Giờ";
static const char* kDtShowYear       = "Hiển thị số Năm";
static const char* kDtShortYear      = "Năm rút gọn (24 thay vì 2024)";
static const char* kDtShowSec        = "Hiển thị số Giây";
static const char* kDtSeparator      = "Kiểu phân cách ngày:";
static const char* kDtSepSlash       = "Dấu chéo (20/11)";
static const char* kDtSepDot         = "Dấu chấm (20.11)";
static const char* kDtSepDash        = "Dấu gạch (20-11)";

// ─── Text Panel Tab ──────────────────────────────────────────────
static const char* kTxtTitle         = "Cấu Hình Văn Bản Tự Do";
static const char* kTxtScroll        = "Cuộn chữ ngang";
static const char* kTxtContent       = "Nội dung hiển thị:";
static const char* kTxtSpeed         = "Tốc độ cuộn (1 - 10):";
static const char* kTxtColor         = "Màu chữ:";

// ─── Auto-Cycle Tab ──────────────────────────────────────────────
static const char* kAutoTitle        = "Kịch Bản Màn Hình";
static const char* kAutoEnable       = "Bật Tự Động Xoay Vòng";
static const char* kAutoNote         = "0: Đếm Ngược, 1: Cảm Biến, 2: Ngày Giờ, 3: Văn Bản.<br>VD: \"0,2,0\" sẽ lặp Đếm Ngược -> Ngày Giờ -> Đếm Ngược.";
static const char* kAutoSeqLabel     = "Thứ tự hiển thị:";
static const char* kAutoTimersTitle  = "Thời gian trình chiếu (giây)";
static const char* kFwTitle          = "Lịch Pháo Hoa";
static const char* kFwNote           = "Cách nhau bởi \";\". VD: 10:35,5;08:40,10";
static const char* kFwLabel          = "Chuỗi lịch pháo hoa:";
static const char* kFwTestBtn        = "Bắn Thử Pháo Hoa";

// ─── OTA Tab ─────────────────────────────────────────────────────
static const char* kOtaTitle         = "Cập Nhật Phần Mềm (OTA)";
static const char* kOtaNote          = "Tải file firmware (.bin) từ PlatformIO để nâng cấp qua WiFi. Cài đặt sẽ được giữ nguyên. Bảng LED sẽ tự khởi động lại.";
static const char* kOtaBtn           = "Bắt Đầu Cập Nhật";
static const char* kOtaUploading     = "Đang tải lên: ";
static const char* kOtaSuccess       = "Hoàn tất! Đang khởi động lại...";
static const char* kOtaError         = "Lỗi mất kết nối!";

// ─── Colors ──────────────────────────────────────────────────────
static const char* kColors[] = {"Xanh lá", "Đỏ", "Xanh lam", "Vàng", "Cyan", "Tím", "Trắng"};

// ─── Restart / Splash pages ──────────────────────────────────────
static const char* kRestartMsg       = "Đang khởi động lại...";
static const char* kRestartNote      = "Vui lòng đợi vài giây và kết nối lại WiFi.";
static const char* kSplashMsg        = "Đang trình diễn Splash Screen...";
static const char* kSplashNote       = "Tự động quay lại sau 10 giây.";

} // namespace WebUI
