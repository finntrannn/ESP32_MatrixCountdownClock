# ESP32 Matrix Clock & Weather Station 🚀

> Một dự án đồng hồ đếm ngược thông minh kết hợp trạm đo thời tiết, hiển thị
> trên màn hình LED Matrix 64x32, được điều khiển bằng ESP32 và cấu hình hoàn
> toàn qua Web.

![ESP32](https://img.shields.io/badge/Platform-ESP32-blue)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C)
![Framework](https://img.shields.io/badge/Framework-PlatformIO%20%2F%20Arduino-orange)
![License](https://img.shields.io/badge/License-MIT-green)

**Tác giả:** finntrannn ([finntrannn.id.vn](https://finntrannn.id.vn))  
**GitHub:** [finntrannn](https://github.com/finntrannn)

---

## ✨ Tính Năng Nổi Bật

### ⏳ Chế Độ Đếm Ngược (Countdown Screen)

- Đếm ngược chính xác Ngày, Giờ, Phút, Giây đến một sự kiện quan trọng (ví dụ:
  Kỳ thi THPT Quốc Gia).
- Dòng chữ chạy (Marquee) tuỳ chỉnh được hiển thị mượt mà.
- Khung viền LED cầu vồng chạy vòng quanh màn hình ấn tượng (Có thể thay đổi tốc
  độ).
- Tuỳ chọn ẩn/hiện bộ đếm giây, tự động căn giữa bố cục hoàn hảo.
- Tự động lấy và đồng bộ thời gian thực qua máy chủ NTP.

### 🌡️ Chế Độ Trạm Thời Tiết (Sensor Screen)

- Đọc và hiển thị Nhiệt độ & Độ ẩm thời gian thực thông qua cảm biến **DHT11**.
- Thanh tiến trình ngang (Progress Bar) mô phỏng mức độ nhiệt/ẩm và tự động đổi
  màu trơn tru theo dải giá trị (ví dụ: nhiệt độ cao sẽ chuyển đỏ, thấp chuyển
  xanh).
- Tích hợp Background Gradient dịu mắt có thể tắt/mở theo ý muốn.
- Animation khởi động (slide-in) mượt mà mỗi khi đổi màn hình.

### ⚙️ Web Configuration Portal (Giao Diện Cấu Hình Qua Web)

- Không cần cắm cáp hay sửa code để thay đổi cài đặt! ESP32 sẽ phát ra WiFi
  Access Point.
- Giao diện **Web UI** mang phong cách Dark Theme cực kỳ hiện đại và phản hồi
  nhanh.
- Tuỳ chỉnh mọi thứ trực tiếp trên điện thoại/máy tính:
    - Chuyển đổi giữa 2 màn hình.
    - Tắt/Mở toàn bộ màn hình LED hoặc chỉnh độ sáng.
    - Thay đổi ngày giờ đếm ngược, thay đổi dòng chữ chạy.
    - Bật/tắt các hiệu ứng đồ hoạ (Chạy viền, Nền Gradient, Đếm giây...).
- Toàn bộ cài đặt được lưu vĩnh viễn vào bộ nhớ **NVS Flash** của mạch.

### 🛠️ Kiến Trúc Phần Mềm Chuẩn Mực (Clean Architecture)

- Codebase được đập đi xây lại 100% theo phương chuẩn **OOP**, phân tách rõ ràng
  thành các class chuyên biệt (`NetworkManager`, `DisplayManager`,
  `WebConfigServer`, `SensorScreen`, `CountdownScreen`...).
- **Tối ưu Multi-Tasking với FreeRTOS**: Các tác vụ chặn (blocking) như đọc cảm
  biến DHT11 (mất tới 500ms) được chuyển sang một Core riêng biệt (Core 0), đảm
  bảo vòng lặp vẽ UI (Core 1) luôn đạt 60FPS+ không bao giờ bị giật lag.

---

## 🛠️ Phần Cứng Yêu Cầu

1. **ESP32** (Sử dụng DOIT ESP32 DevKit V1 hoặc các board tương đương).
2. **Màn hình LED Matrix HUB75** - Kích thước 64x32 (Pixel Pitch tuỳ chọn như
   P3, P4, P5).
3. Cảm biến nhiệt độ, độ ẩm **DHT11**.
4. Cáp kết nối và bộ nguồn rời (Màn hình LED Matrix dòng tiêu thụ rất lớn, cần
   nguồn 5V - 5A trở lên).

---

## 🚀 Cài Đặt & Chạy Dự Án

### 1. Chuẩn Bị Môi Trường

- Tải và cài đặt [VSCode](https://code.visualstudio.com/).
- Cài đặt extension **PlatformIO** trong VSCode.

### 2. Tải Mã Nguồn

```bash
git clone https://github.com/finntrannn/ESP32-Matrix-Clock.git
cd ESP32-Matrix-Clock
```

### 3. Cấu Hình Chân GPIO

Mở file `include/Config.h`, kiểm tra và cấu hình các chân cắm sao cho khớp với
sơ đồ nối dây thực tế của bạn. Mặc định:

- Cảm biến DHT11 kết nối vào chân `GPIO 32`.

_(Lưu ý: Chân cắm của Panel LED được tự động định cấu hình thông qua thư viện
`ESP32-HUB75-MatrixPanel-I2S-DMA` dựa trên hardware mặc định của thư viện)._

### 4. Build và Upload

Cắm cáp ESP32 vào máy tính và chạy lệnh sau, PlatformIO sẽ tự động tải thư viện
và nạp code:

```bash
pio run -t upload
```

---

## 🌐 Hướng Dẫn Sử Dụng Web Config

Sau khi cấp nguồn cho mạch thành công:

1. Mạch sẽ tự động ghi nhớ và chạy các thiết lập cũ. Nếu là lần chạy đầu tiên,
   màn hình hiển thị chế độ Đếm Ngược nhưng có thể sai giờ do chưa kết nối WiFi.
2. Dùng điện thoại bắt WiFi do mạch ESP32 phát ra:
    - **Tên WiFi (SSID):** `THPT_QG_Config`
    - **Mật khẩu:** `12345678`
3. Mở trình duyệt web, truy cập vào địa chỉ mặc định của Access Point (thường sẽ
   tự động pop-up).
4. Thực hiện cài đặt thông số và ấn **Lưu**. Giao diện sẽ tự động cập nhật ngay
   trên màn hình LED.

---

## 📚 Thư Viện Sử Dụng (Dependencies)

Dự án được xây dựng dựa trên các thư viện cực kỳ nhẹ và tối ưu (PlatformIO sẽ tự
tải):

- `mrfaptastic/ESP32 HUB75 LED MATRIX PANEL DMA Display`
- `adafruit/Adafruit GFX Library`
- `adafruit/DHT sensor library`

---

## 📝 Giấy Phép (License)

Dự án này được chia sẻ dưới giấy phép **MIT**. Bạn hoàn toàn có thể tự do sao
chép, chỉnh sửa và ứng dụng cho dự án cá nhân!

---

> Mã nguồn bởi **finntrannn** - Cảm ơn các bạn đã ghé thăm repository!
