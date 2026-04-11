# ESP32 Matrix Countdown Clock

Dự án đồng hồ đếm ngược kết hợp trạm đo nhiệt độ, độ ẩm mini. Project này được
thiết kế để chạy trên màn hình LED Matrix 64x32 (HUB75) điều khiển bằng ESP32,
và toàn bộ thông số đều có thể cấu hình được qua giao diện Web rất tiện lợi.

![ESP32](https://img.shields.io/badge/Platform-ESP32-blue)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C)
![Framework](https://img.shields.io/badge/Framework-PlatformIO%20%2F%20Arduino-orange)
![License](https://img.shields.io/badge/License-MIT-green)

**Tác giả:** finntrannn ([finntrannn.id.vn](https://finntrannn.id.vn))  
**GitHub:** [finntrannn](https://github.com/finntrannn)

---

<div align="center">
  <img src="demo/demo_1.jpg" alt="Demo 1" width="32%">
  <img src="demo/demo_2.jpg" alt="Demo 2" width="32%">
  <img src="demo/demo_3.jpg" alt="Demo 3" width="32%">
</div>

---

## Mục Lục

- [Tính Năng Chính](#tính-năng-chính)
- [Chuẩn Bị Phần Cứng](#chuẩn-bị-phần-cứng)
- [Cài Đặt Code](#cài-đặt-code)
    - [1. Chuẩn bị môi trường](#1-chuẩn-bị-môi-trường)
    - [2. Tải Source Code](#2-tải-source-code)
    - [3. Cấu hình phần cứng (Nếu dùng pinout khác)](#3-cấu-hình-phần-cứng-nếu-dùng-pinout-khác)
    - [4. Upload Firmware](#4-upload-firmware)
- [Cấu Hình Bảng Mạch Qua Web](#cấu-hình-bảng-mạch-qua-web)
- [License](#license)

---

## Tính Năng Chính

- **Đồng hồ đếm ngược:** Đếm lùi thời gian (Ngày, Giờ, Phút, Giây) đến một sự
  kiện quan trọng (trong project hiện tại là kỳ thi THPT Quốc Gia).
- **Xem nhiệt độ, độ ẩm:** Tích hợp đọc cảm biến DHT11 để hiển thị thời tiết
  thực tế trong phòng.
- **Tự động chuyển màn hình (Auto-Cycle):** Kịch bản lặp lại các màn hình hiển
  thị tuỳ ý (Đếm ngược -> Cảm biến -> Lịch -> Văn bản). Có thể lên lịch cố định
  cho hiệu ứng bắn pháo hoa theo thời gian thực tế.
- **Web UI & Captive Portal:** Khi kết nối vào điện thoại, web thiết lập sẽ tự
  bật lên. Có thể thay đổi ngày thì, tốc độ chạy chữ, bật tắt phần viền, đổi màu
  hay chèn lịch pháo hoa mà không cần đụng 1 dòng code. Hỗ trợ cập nhật phần mềm
  không dây (OTA) cũng ngay trực tiếp trên điện thoại.
- **Tối ưu mượt mà 60FPS:** Code đã được viết đa luồng chạy ngầm bằng FreeRTOS.
  Tác vụ nặng (kết nối ESPAsyncWebServer, đọc cảm biến...) đẩy qua Core 1, để
  dành Core 0 thuần túy cho việc quét đồ hoạ ra cổng DMA giúp không bị bóng ma
  viền ảnh hay khựng giật.

---

## Chuẩn Bị Phần Cứng

Để thực hiện project này, bạn sẽ cần:

1. **Mạch vi điều khiển ESP32** (Board DOIT ESP32 DevKit V1 chuẩn 30/38 chân).
2. **Màn hình LED Matrix HUB75:** Kích thước 64x32 (Đang sử dụng với P5, có thể
   sử dụng P4, P3,...).
3. **Cảm biến:** Nhiệt độ, độ ẩm DHT11.
4. **Nguồn điện:** Cáp xịn chống nhiễu nhiễu và module nguồn nuôi rời cho màn
   hình LED Matrix (màn kéo dòng rất khủng, nhất định phải dùng nguồn 5V - 5A
   trở lên thì LED mới không bị sụp).

---

## Cài Đặt Code

### 1. Chuẩn bị môi trường

Project này viết trên nền Arduino C++ mở rộng qua hệ sinh thái **PlatformIO**.
Hãy tải [VSCode](https://code.visualstudio.com/), sau đó tìm và cài extension
PlatformIO vào trong VSCode.

### 2. Tải Source Code

Mở terminal chạy lệnh:

```bash
git clone https://github.com/finntrannn/ESP32_MatrixCountdownClock.git
cd ESP32_MatrixCountdownClock
```

### 3. Cấu hình phần cứng (Nếu dùng pinout khác)

Vào file `include/Config.h`, kiểm tra và cấu hình các chân cắm sao cho chuẩn với
cách bạn đấu dây vào ESP32.

- Mặc định DHT11 đang được trỏ vào chân `GPIO 32`.
- Chân nối từ ESP32 đến Hub75 được nối theo chuẩn sơ đồ pin mặc định của thư
  viện `ESP32-HUB75-MatrixPanel-I2S-DMA`.

### 4. Upload Firmware

Cắm cáp kết nối ESP32 với máy tính và chạy command:

```bash
pio run -t upload
```

Đợi xíu để PlatformIO tự cài cắm các thư viện cần thiết và nạp xuống mạch là
xong.

---

## Cấu Hình Bảng Mạch Qua Web

Lần đầu khởi động, ESP32 sẽ tự phát ra một mạng WiFi tên là `THPT_QG_Config` với
password: `12345678`.

1. Dùng điện thoại của bạn bắt WiFi tên trên.
2. Web cài đặt sẽ tự chạy lên, nếu không thì tự vào trình duyệt gõ
   `192.168.4.1`.
3. Nhập WiFi internet nhà bạn vào để mạch sync lấy giờ NTP, cài ngày giờ dự thi
   mục tiêu, tốc độ lướt chữ... rồi bấm **Lưu Cấu Hình**.
4. Toàn bộ thiết lập này được lưu thẳng vào bộ nhớ ROM vĩnh viễn và mạch sẽ tự
   thay đổi tuỳ chọn cấu hình ra ngoài LED Matrix.

### Các tính năng có thể tùy chỉnh:

Giao diện Web UI cung cấp khả năng điều khiển bao quát hầu hết các tính năng hoạt động của dự án mà không cần nạp lại code:

- **Cài đặt Mạng & Hệ thống:**
  - Cấu hình WiFi gia đình để mạch tự động đồng bộ giờ qua Internet (NTP).
  - Đổi tên (SSID) và Mật khẩu Access Point phát ra từ ESP32.
  - Cập nhật phần mềm không dây (OTA Update) nâng cấp firmware từ xa.
- **Chế độ Màn hình & Hiệu ứng lặp:**
  - Chuyển màn hình thủ công (Đếm ngược, Cảm biến, Lịch, Cuộn chữ, Random Spinner).
  - Thiết lập vòng lặp (Auto-Cycle): cài đặt thời gian dừng ở từng màn hình, hoặc loại bỏ các màn hình không muốn hiển thị vòng lặp.
- **Custom màn hình Đếm Ngược:**
  - Chọn ngày, giờ đích đến (Target Time) dễ dàng bằng giao diện đồ hoạ.
  - Bật/tắt đếm giây, bật/tắt năm, ngăn cách hiển thị theo định dạng `/`, `.`, `-`.
  - Thay đổi màu sắc Số Ngày và Thời gian bằng bộ chọn màu (Color Picker).
  - Tích hợp thay đổi câu chúc, lời động viên phía dưới.
- **Giao diện & Hiệu ứng Khác:**
  - Điều chỉnh độ sáng tổng màn hình LED.
  - Màn hình chữ cuộn mát-trận (Text Matrix): Tự gõ văn bản, chỉnh cỡ chữ, tốc độ và màu sắc, cuộn theo mọi hướng.
  - Bật/tắt thanh LED viền cầu vồng (Rainbow border) quanh đồng hồ, điều chỉnh vận tốc nhấp nháy viền.
  - Bật/tắt hiển thị màn hình khởi động (Splash Screen), tùy ý thay đổi nội dung chữ chào mừng.
  - Bật/tắt icon trạng thái sóng WiFi.
- **Tiện ích Giải trí:**
  - Chế độ chọn số ngẫu nhiên (Random Spinner): Thiết lập Max/Min, bộ thời gian lướt số để gọi số chơi lô tô, bốc thăm may mắn trong các dịp lễ tết.
  - Chế độ lập lịch pháo hoa (Fireworks Schedule): Hẹn giờ bắn pháo hoa độc lập liên tục trong một khoảng giờ cụ thể theo thời gian thực tế.

---

## License

Mã nguồn được phân phối với giấy phép **MIT**.
