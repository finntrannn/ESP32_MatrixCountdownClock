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

---

## License

Mã nguồn được phân phối với giấy phép **MIT**.
