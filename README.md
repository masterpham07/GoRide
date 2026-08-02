# GoRide — Ứng dụng đặt xe công nghệ (C++ & Qt)

GoRide là bài tập lớn môn C++, mô phỏng nền tảng đặt xe công nghệ (tương tự Grab, Be, Gojek). Ứng dụng sở hữu giao diện đồ hoạ (GUI) trực quan được xây dựng bằng framework **Qt 6**, tích hợp bản đồ dạng lưới (Grid Map) mô phỏng quá trình di chuyển của tài xế và tìm đường đi tối ưu theo thời gian thực.

## 1. Tính năng nổi bật

- **Giao diện trực quan (GUI)**: Chuyển đổi hoàn toàn từ nền tảng Console sang giao diện đồ hoạ hiện đại sử dụng thư viện Qt Widgets.
- **Bản đồ tương tác**: Bản đồ lưới mô phỏng đường đi với các icon trực quan định dạng SVG cho Người dùng, Tài xế, Điểm đến và các Chướng ngại vật.
- **Định tuyến thông minh (Pathfinding)**: Thuật toán tìm đường đi tối ưu, tự động nhận diện và né tránh kẹt xe/chướng ngại vật trên bản đồ để đảm bảo thời gian di chuyển nhanh nhất (tối ưu hóa khoảng cách Manhattan).
- **Hệ thống đặt xe linh hoạt**: 
  - Tuỳ chọn loại phương tiện (Xe máy, Xe 4 chỗ, Xe 7 chỗ...).
  - Tự động ghép cuốc với tài xế gần nhất hoặc cho phép người dùng chủ động chọn tài xế theo đánh giá (số sao).
  - Tính toán quãng đường và ước lượng giá cước (áp dụng hệ số nhân giá khi vào giờ cao điểm).
- **Thanh toán đa dạng**: Xử lý thanh toán qua ví điện tử hoặc tiền mặt. Hệ thống tự động ghi nhận và lưu trữ lịch sử chuyến đi của người dùng.

## 2. Cách Build & Chạy ứng dụng

Dự án sử dụng hệ thống build **CMake** và yêu cầu framework **Qt 6** (khuyến nghị sử dụng trình biên dịch MinGW 64-bit).

### 2.1. Build bằng Qt Creator (Khuyến nghị cho Developer)
1. Mở phần mềm Qt Creator, chọn `File` -> `Open File or Project...` và trỏ tới file `CMakeLists.txt` trong thư mục gốc của dự án.
2. Cấu hình kit build là **Desktop Qt 6.x.x MinGW 64-bit**.
3. Nhấn nút **Run** (hoặc tổ hợp phím `Ctrl + R`) để CMake tự động sinh file build và khởi chạy ứng dụng.

### 2.2. Chạy trực tiếp Ứng dụng (Dành cho Giảng viên / Người dùng)
Nếu chỉ cần xem ứng dụng hoạt động mà không cài đặt môi trường code, vui lòng truy cập mục **Releases** của repository này, tải xuống file nén `GoRide.zip`, giải nén và nhấp đúp vào file `GoRide.exe` để chạy.

## 3. Cấu trúc thư mục

~~~text
GoRide/
├── assets/                 # Chứa tài nguyên giao diện (Media/Assets)
│   ├── icons/              # Các file icon định dạng SVG (airport, car, driver...)
│   └── styles/             # File goride.qss cấu hình CSS/Style cho giao diện
├── docs/                   # Chứa file Word báo cáo chi tiết của dự án
├── slides/                 # Chứa file PowerPoint thuyết trình báo cáo
├── src/                    # Mã nguồn C++ chính của phần mềm
│   ├── models/             # Định nghĩa các thực thể dữ liệu tĩnh (User, Place, Trip...)
│   ├── services/           # Xử lý logic nghiệp vụ lõi (MatchingEngine, PathFinder, Payment...)
│   ├── ui/                 # Mã nguồn giao diện Qt (MainWindow, MapWidget...)
│   └── utils/              # Các class tiện ích hỗ trợ (đọc/ghi file CSV với FileHandler...)
├── CMakeLists.txt          # File cấu hình build dự án bằng CMake
└── README.md               # Tài liệu giới thiệu dự án
~~~

## 4. Kiến trúc & Công nghệ áp dụng

- **Ngôn ngữ & Framework**: C++17, Qt 6.11.1
- **Kiến trúc phần mềm**: Áp dụng triệt để các tính chất cốt lõi của Lập trình hướng đối tượng (OOP): Đóng gói, Kế thừa, Đa hình, Trừu tượng. Tuân thủ nguyên tắc Tách biệt trách nhiệm (Separation of Concerns), phân chia rõ ràng giữa Giao diện (UI) và Logic nghiệp vụ.
- **Thuật toán cốt lõi**: Sử dụng thuật toán duyệt đồ thị (Graph Traversal) trên ma trận để giải quyết bài toán Tìm đường đi ngắn nhất (Shortest Path) có né vật cản.

## 5. Phân công công việc nhóm

Dự án được phân chia module rõ ràng, đảm bảo quy trình làm việc hiệu quả và đồng đều cho cả 5 thành viên thông qua Git/GitHub:

| Thành viên | Trách nhiệm chính trong dự án |
|:---|:---|
| Phạm Ngọc Thắng | Tích hợp tài nguyên giao diện (UI/UX Assets, Stylesheets), kiểm thử luồng ứng dụng và viết tài liệu báo cáo kỹ thuật. |
| Nguyễn Phương Nam | Xây dựng các lớp dữ liệu nền tảng (Data Models), chuẩn hóa luồng dữ liệu và viết tài liệu báo cáo kỹ thuật. |
| Phạm Đan Trường | Phát triển các thuật toán và dịch vụ lõi (PathFinder, MatchingEngine, Cước phí, Dịch vụ thanh toán). |
| Trần Minh Khôi | Phát triển Giao diện đồ hoạ chính (UI/UX), lập trình và tích hợp bản đồ trực quan (MapWidget) với framework Qt. |
| Phan Minh Tân | Phát triển mô-đun quản lý File (FileHandler), cấu hình CMake, quản lý repository và thiết kế Slides thuyết trình. |

---
*Chân thành cảm ơn Thầy/Cô đã dành thời gian xem xét dự án của nhóm!*
