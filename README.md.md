# GoRide — Ứng dụng đặt xe công nghệ bằng C++ và Qt

GoRide là bài tập lớn môn C++, mô phỏng quy trình hoạt động của một nền tảng đặt xe công nghệ như Grab, Be hoặc Gojek. Ứng dụng được phát triển bằng **C++17** và **Qt 6**, sử dụng giao diện đồ họa Qt Widgets, bản đồ dạng lưới và các thuật toán xử lý đường đi để mô phỏng quá trình đặt xe, ghép tài xế, di chuyển, thanh toán và lưu lịch sử chuyến đi.

---

## 1. Tính năng chính

### 1.1. Giao diện đồ họa bằng Qt Widgets

- Giao diện trực quan, thay thế hoàn toàn phiên bản console ban đầu.
- Hiển thị thông tin người dùng, số dư ví và trạng thái chuyến đi.
- Phân chia rõ khu vực điều khiển và khu vực bản đồ.
- Sử dụng stylesheet `.qss` và icon `.svg` để đồng bộ giao diện.

### 1.2. Bản đồ dạng lưới tương tác

- Mô phỏng bản đồ bằng các ô lưới.
- Hiển thị vị trí tài xế, hành khách, điểm đón, điểm đến và vật cản.
- Hỗ trợ chọn địa điểm từ danh sách có sẵn.
- Hỗ trợ chọn trực tiếp điểm đón và điểm đến trên bản đồ.
- Mô phỏng quá trình tài xế di chuyển theo đường đi đã tìm được.

### 1.3. Đặt xe và ghép tài xế

- Cho phép người dùng chọn loại phương tiện.
- Hiển thị danh sách tài xế phù hợp.
- Hỗ trợ tự động chọn tài xế hoặc lựa chọn tài xế thủ công.
- Có thể ưu tiên tài xế dựa trên khoảng cách và mức đánh giá.
- Cập nhật trạng thái theo từng giai đoạn của chuyến đi.

### 1.4. Tính giá cước

Giá chuyến đi được ước lượng dựa trên các yếu tố như:

- Loại phương tiện.
- Quãng đường di chuyển.
- Mức giá cơ bản.
- Hệ số phụ thu hoặc hệ số giờ cao điểm nếu được áp dụng.

Phần tính giá được tách khỏi giao diện để thuận tiện cho việc kiểm thử và mở rộng.

### 1.5. Thanh toán

- Hỗ trợ thanh toán bằng tiền mặt hoặc ví điện tử.
- Kiểm tra số dư trước khi thanh toán bằng ví.
- Tự động cập nhật số dư sau khi chuyến đi hoàn thành.
- Hiển thị thông báo khi số dư không đủ.

### 1.6. Lưu lịch sử chuyến đi

- Lịch sử chuyến đi được lưu trong file `data/trips.csv`.
- File CSV chỉ được tạo khi người dùng hoàn thành chuyến đi đầu tiên.
- Nếu file chưa tồn tại, chương trình sẽ tự động tạo file.
- Dữ liệu phát sinh trong quá trình chạy không được đưa lên Git.
- Thư mục `data/` được giữ trong repository bằng file `.gitkeep`.

---

## 2. Công nghệ sử dụng

| Thành phần | Công nghệ |
|---|---|
| Ngôn ngữ | C++17 |
| Framework giao diện | Qt 6.11.1 |
| Bộ công cụ giao diện | Qt Widgets |
| Hệ thống build | CMake |
| Trình biên dịch khuyến nghị | MinGW 64-bit |
| Lưu trữ dữ liệu | CSV |
| Quản lý mã nguồn | Git và GitHub |
| Tài nguyên giao diện | SVG, QSS, Qt Resource System |

---

## 3. Yêu cầu hệ thống

### Dành cho developer

- Qt Creator.
- Qt 6.x.
- Kit Desktop Qt MinGW 64-bit.
- CMake.
- Trình biên dịch hỗ trợ C++17.
- Git nếu cần clone hoặc đóng góp mã nguồn.

### Dành cho người dùng

Người dùng không cần cài Qt hoặc trình biên dịch nếu sử dụng bản đã đóng gói trong mục **Releases**.

---

## 4. Cách build và chạy ứng dụng

### 4.1. Build bằng Qt Creator

1. Mở Qt Creator.
2. Chọn `File` → `Open File or Project...`.
3. Chọn file `CMakeLists.txt` ở thư mục gốc.
4. Chọn kit `Desktop Qt 6.x.x MinGW 64-bit`.
5. Chờ Qt Creator cấu hình CMake.
6. Nhấn `Ctrl + B` để build.
7. Nhấn `Ctrl + R` để chạy ứng dụng.

### 4.2. Build bằng dòng lệnh

Ví dụ với môi trường đã cấu hình Qt và CMake:

```bash
cmake -S . -B build
cmake --build build
```

File thực thi sẽ được tạo trong thư mục build tương ứng với kit và cấu hình được sử dụng.

### 4.3. Chạy bản Release

1. Truy cập mục **Releases** của repository.
2. Tải file `GoRide.zip`.
3. Giải nén toàn bộ file.
4. Giữ nguyên cấu trúc thư mục sau khi giải nén.
5. Chạy `GoRide.exe`.

> Không nên tách riêng file `GoRide.exe` khỏi các thư viện và thư mục Qt đi kèm vì ứng dụng có thể không khởi động được.

---

## 5. Hướng dẫn sử dụng nhanh

Quy trình sử dụng cơ bản:

1. Khởi động ứng dụng.
2. Chọn điểm đón.
3. Chọn điểm đến.
4. Chọn loại phương tiện.
5. Chọn phương thức ghép tài xế.
6. Xác nhận đặt xe.
7. Chờ tài xế di chuyển đến điểm đón.
8. Bắt đầu chuyến đi.
9. Hoàn thành chuyến đi.
10. Thanh toán.
11. Xem lịch sử chuyến đi.

---

## 6. Cấu trúc thư mục

```text
GoRide/
├── assets/          # Chứa tài nguyên giao diện (icons, styles...)
│   ├── icons/       # Các icon .svg (car, driver, passenger, mall...)
│   └── styles/      # File .qss định nghĩa style cho giao diện Qt
│
├── include/         # Chứa các file header (.h) định nghĩa cấu trúc và thư viện
│   ├── data/        # Header xử lý đọc/ghi file
│   ├── models/      # Header các struct/class dữ liệu (User, Trip, Map...)
│   ├── services/    # Header logic nghiệp vụ (MatchingEngine, PathFinder...)
│   └── ui/          # Header giao diện (console + qt)
│
├── resources/       # File .qrc quản lý tài nguyên Qt (biên dịch vào app)
│
├── src/             # Mã nguồn C++ chính và các thành phần giao diện (UI)
│   ├── data/        # Cài đặt đọc/ghi file
│   ├── models/      # Cài đặt các struct/class dữ liệu
│   ├── services/    # Cài đặt logic nghiệp vụ lõi
│   ├── ui/          # Cài đặt giao diện (console + qt)
│   └── main.cpp     # Điểm khởi chạy chương trình
│
├── data/            # Thư mục lưu dữ liệu cục bộ
│   └── .gitkeep     # Giữ thư mục rỗng trên Git; trips.csv được sinh khi chạy app
│
├── CMakeLists.txt   # File cấu hình build dự án bằng CMake
├── .gitignore       # File cấu hình bỏ qua các file build/app không cần thiết
└── README.md        # Tài liệu giới thiệu dự án
```

---

## 7. Kiến trúc phần mềm

Dự án được tổ chức theo nguyên tắc tách biệt trách nhiệm:

- **Models**: Chứa các lớp và cấu trúc dữ liệu như người dùng, chuyến đi, tài xế, vị trí và bản đồ.
- **Services**: Chứa logic nghiệp vụ như tìm đường, ghép tài xế, tính giá cước và thanh toán.
- **Data**: Chịu trách nhiệm đọc và ghi dữ liệu CSV.
- **UI**: Xử lý giao diện console và giao diện Qt.
- **Resources**: Quản lý icon, stylesheet và các tài nguyên được nhúng vào ứng dụng.
- **main.cpp**: Khởi tạo `QApplication`, tạo cửa sổ chính và bắt đầu vòng lặp sự kiện của Qt.

Việc tách các module giúp:

- Hạn chế đặt logic nghiệp vụ trực tiếp trong lớp giao diện.
- Dễ sửa lỗi và bảo trì.
- Dễ phân chia công việc cho nhiều thành viên.
- Dễ mở rộng thêm loại xe, phương thức thanh toán hoặc thuật toán mới.
- Giảm sự phụ thuộc giữa các thành phần.

---

## 8. Thuật toán và xử lý cốt lõi

### 8.1. Biểu diễn bản đồ

Bản đồ được mô hình hóa dưới dạng ma trận hai chiều. Mỗi ô có thể đại diện cho:

- Đường có thể di chuyển.
- Vật cản hoặc khu vực không thể đi qua.
- Vị trí tài xế.
- Điểm đón.
- Điểm đến.
- Địa điểm đặc biệt.

Mô hình này cho phép chuyển bài toán tìm đường thành bài toán duyệt đồ thị trên lưới.

### 8.2. Tìm đường đi ngắn nhất

Thuật toán duyệt theo chiều rộng, thường được triển khai bằng **Breadth-First Search (BFS)**, được sử dụng để tìm đường đi ngắn nhất trên bản đồ lưới không trọng số.

Quy trình cơ bản:

1. Đưa ô bắt đầu vào hàng đợi.
2. Đánh dấu ô đã được duyệt.
3. Lần lượt kiểm tra các ô lân cận hợp lệ.
4. Bỏ qua ô nằm ngoài bản đồ hoặc ô chứa vật cản.
5. Lưu lại ô cha của mỗi ô được thăm.
6. Khi đến đích, truy vết ngược để dựng lại đường đi.
7. Chuyển đường đi thành chuỗi vị trí phục vụ animation.

Với bản đồ gồm `V` ô hợp lệ và `E` liên kết giữa các ô, độ phức tạp của BFS là:

```text
O(V + E)
```

Trên bản đồ lưới, mỗi ô chỉ có một số lượng nhỏ ô kề nên thuật toán phù hợp với quy mô mô phỏng của ứng dụng.

### 8.3. Khoảng cách Manhattan

Khoảng cách Manhattan có thể được sử dụng để ước lượng khoảng cách giữa hai vị trí trên lưới:

```text
distance = |x1 - x2| + |y1 - y2|
```

Cách tính này phù hợp khi phương tiện chỉ di chuyển theo bốn hướng cơ bản trên bản đồ.

### 8.4. Ghép tài xế

Quá trình ghép tài xế có thể xem xét các tiêu chí:

- Trạng thái đang rảnh.
- Khoảng cách từ tài xế đến điểm đón.
- Loại phương tiện.
- Mức đánh giá.
- Lựa chọn ưu tiên của người dùng.

Hệ thống lọc danh sách tài xế hợp lệ trước, sau đó lựa chọn tài xế phù hợp hoặc hiển thị danh sách để người dùng tự chọn.

### 8.5. Mô phỏng di chuyển

Sau khi tìm được đường đi:

1. Đường đi được lưu dưới dạng danh sách tọa độ.
2. `QTimer` phát tín hiệu theo từng khoảng thời gian.
3. Mỗi tín hiệu cập nhật vị trí xe sang ô tiếp theo.
4. Bản đồ được vẽ lại để thể hiện trạng thái mới.
5. Khi xe đến điểm đón hoặc điểm đến, trạng thái chuyến đi được cập nhật.

Cách triển khai này giúp phần animation không làm khóa giao diện.

### 8.6. Lưu dữ liệu CSV

Khi chuyến đi hoàn thành:

1. Chương trình kiểm tra thư mục `data/`.
2. Nếu chưa có, thư mục được tạo.
3. File `trips.csv` được mở ở chế độ ghi nối tiếp.
4. Nếu file chưa tồn tại, hệ thống tự tạo file.
5. Thông tin chuyến đi được ghi thành một dòng CSV.
6. Lịch sử có thể được đọc lại để hiển thị trên giao diện.

File CSV là dữ liệu phát sinh cục bộ nên được loại khỏi Git bằng `.gitignore`.

---

## 9. Nguyên tắc quản lý dữ liệu và Git

Repository chỉ lưu mã nguồn và tài nguyên cần thiết để build dự án.

Không đưa các nội dung sau lên Git:

```text
build/
.qtcreator/
*.exe
*.dll
data/*.csv
```

Thư mục `data/` vẫn được giữ bằng:

```text
data/.gitkeep
```

Ví dụ `.gitignore`:

```gitignore
build/
.qtcreator/
*.exe
*.dll
data/*.csv
!data/.gitkeep
```

---

## 10. Phân công công việc nhóm

Dự án được phân chia theo từng module nhằm đảm bảo trách nhiệm rõ ràng, hạn chế xung đột mã nguồn và thuận tiện theo dõi đóng góp của từng thành viên trên Git/GitHub.

| STT | Thành viên | Phần việc phụ trách |
|:---:|---|---|
| 1 | **Phạm Ngọc Thắng** | Tích hợp tài nguyên giao diện (**UI/UX Assets, Stylesheets**), kiểm thử luồng ứng dụng và viết tài liệu báo cáo kỹ thuật. |
| 2 | **Nguyễn Phương Nam** | Xây dựng các lớp dữ liệu nền tảng (**Data Models**), chuẩn hóa luồng dữ liệu và viết tài liệu báo cáo kỹ thuật. |
| 3 | **Phạm Đan Trường** | Phát triển các thuật toán và dịch vụ lõi: **PathFinder, MatchingEngine, tính cước phí và dịch vụ thanh toán**. |
| 4 | **Trần Minh Khôi** | Phát triển giao diện đồ họa chính (**UI/UX**), lập trình và tích hợp bản đồ trực quan (**MapWidget**) bằng framework Qt. |
| 5 | **Phan Minh Tân** | Phát triển mô-đun quản lý file (**FileHandler**), cấu hình CMake, quản lý repository và thiết kế slide thuyết trình. |
---

## 11. Hướng phát triển

Một số chức năng có thể được mở rộng trong tương lai:

- Đăng nhập và quản lý nhiều người dùng.
- Lưu dữ liệu bằng SQLite thay cho CSV.
- Hỗ trợ nhiều bản đồ và kích thước bản đồ khác nhau.
- Thêm nhiều tiêu chí ghép tài xế.
- Mô phỏng giao thông theo thời gian thực.
- Bổ sung chức năng hủy chuyến và đánh giá tài xế.
- Viết unit test cho các module service.
- Đóng gói ứng dụng cho nhiều hệ điều hành.

---

## 12. Giấy phép và mục đích sử dụng

Dự án được xây dựng phục vụ mục đích học tập và trình bày bài tập lớn môn C++.

---

*Chân thành cảm ơn Thầy/Cô đã dành thời gian xem xét dự án của nhóm!*
