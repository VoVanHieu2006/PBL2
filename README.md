# PBL2 - 2D Adventure Game with C++ & SDL2

## Giới thiệu

**PBL2** là một game phiêu lưu 2D được xây dựng bằng **C++** và **SDL2**. Người chơi điều khiển nhân vật trong bản đồ dạng tilemap, thu thập sách, mở rương, quản lý túi đồ, vượt qua giới hạn bước đi và bước vào màn chiến đấu với boss.

Dự án được thực hiện với mục tiêu luyện tập lập trình hướng đối tượng, xử lý đồ họa 2D, âm thanh, tilemap, va chạm, input người dùng và áp dụng thuật toán tìm đường/tối ưu lộ trình trong game.

## Demo

https://github.com/user-attachments/assets/a75dcbf6-7ddb-4963-a4d9-9772a38244dd


## Tính năng chính

* Menu chính và menu chọn độ khó: Dễ, Trung bình, Khó.
* Điều khiển nhân vật bằng bàn phím.
* Bản đồ 2D sử dụng tilemap từ file `.tmj` và `.tsj`.
* Nhân vật có animation theo hướng di chuyển.
* Sinh sách ngẫu nhiên trên bản đồ.
* Cơ chế nhặt sách và lưu vào túi đồ.
* Hiển thị túi đồ và danh sách sách đã thu thập.
* Mini-game mở rương bằng cách nhập tên sách.
* Gợi ý tên sách bằng cấu trúc dữ liệu Trie.
* Cơ chế giới hạn số bước đi.
* Tính toán lộ trình tối ưu để thu thập sách.
* Mở/khóa cửa theo tiến trình chơi.
* Khu vực boss và màn chơi boss riêng.
* Nhạc nền và hiệu ứng âm thanh cho menu, gameplay, nhặt sách, mở rương, boss fight.

## Công nghệ sử dụng

* **C++**
* **SDL2**
* **SDL2_image**
* **SDL2_ttf**
* **SDL2_mixer**
* **nlohmann/json**
* **Tiled Map Editor** cho dữ liệu bản đồ `.tmj`, `.tsj`
* **MinGW / g++** để biên dịch trên Windows

## Cấu trúc thư mục

```text
PBL2/
├── audio/              # Nhạc nền và hiệu ứng âm thanh
├── img/                # Hình ảnh nhân vật, background, sprite
├── map/                # Tilemap và tileset: .tmj, .tsj, ảnh map
├── phongchu/           # Font chữ dùng trong game
├── src/                # Thư viện SDL2 include/lib
├── AmThanh.cpp/.h      # Quản lý âm thanh
├── BanDoGame.cpp/.h    # Load map, tileset, xử lý tile, cửa, boss area
├── DoiTuong.cpp/.h     # Đối tượng đồ họa cơ bản
├── HamChinh.cpp        # Entry point của game
├── HamChung.cpp/.h     # Hằng số, biến toàn cục, include chung
├── MenuGame.cpp/.h     # Menu chính và chọn độ khó
├── NhanVatGame.cpp/.h  # Nhân vật, di chuyển, nhặt sách, túi đồ, tương tác
├── RuongDo.cpp/.h      # Mini-game rương đồ
├── ThuatToan1.cpp/.h   # BFS, tối ưu lộ trình, 2-opt
├── ThuatToan2.cpp/.h   # Trie gợi ý tên sách
├── ManChoiBoss.cpp/.h  # Màn chơi boss
├── Boss.cpp/.h         # Boss
├── KhiDot.cpp/.h       # Enemy phụ
├── PhapSu.cpp/.h       # Enemy phụ
└── Makefile            # Script build bằng g++
```

## Cài đặt và chạy game

### 1. Clone repository

```bash
git clone https://github.com/VoVanHieu2006/PBL2.git
cd PBL2
```

### 2. Build bằng Makefile

Nếu máy đã có `make` hoặc `mingw32-make`:

```bash
mingw32-make
```

Hoặc:

```bash
make
```

Sau khi build thành công, chương trình sẽ tạo file chạy:

```bash
HamChinh.exe
```

### 3. Chạy game

```bash
./HamChinh.exe
```

Hoặc double click trực tiếp vào `HamChinh.exe` trên Windows.

## Lệnh build thủ công

Nếu không dùng Makefile, có thể build bằng lệnh:

```bash
g++ ^
-I src/include/SDL2 -L src/lib ^
-o HamChinh ^
HamChinh.cpp DoiTuong.cpp BanDoGame.cpp NhanVatGame.cpp RuongDo.cpp MenuGame.cpp AmThanh.cpp ^
ThuatToan1.cpp ThuatToan2.cpp ManChoiBoss.cpp NVTrongBoss.cpp Boss.cpp KhiDot.cpp PhapSu.cpp ^
-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

> Lưu ý: cần để các file `.dll` của SDL2 cùng thư mục với file `.exe` hoặc trong PATH của hệ thống.

## Cách chơi

| Phím / thao tác | Chức năng                 |
| --------------- | ------------------------- |
| Chuột trái      | Chọn menu, chọn độ khó    |
| Mũi tên         | Di chuyển nhân vật        |
| `I`             | Mở / đóng túi đồ          |
| `E`             | Tương tác / mở rương      |
| `ESC`           | Đóng một số giao diện phụ |

## Luồng chơi cơ bản

1. Người chơi vào menu chính.
2. Chọn độ khó.
3. Game load bản đồ và sinh sách ngẫu nhiên.
4. Người chơi di chuyển trong bản đồ để thu thập sách.
5. Khi nhặt sách, sách được thêm vào túi đồ.
6. Người chơi có thể mở túi đồ để xem danh sách sách.
7. Mini-game rương yêu cầu nhập tên sách.
8. Khi hoàn thành điều kiện, cửa được mở.
9. Người chơi đi tới khu vực boss.
10. Game chuyển sang màn chiến đấu boss.

## Thuật toán nổi bật

### BFS trên lưới

Game sử dụng BFS để tính khoảng cách giữa các ô trong bản đồ. BFS phù hợp vì bản đồ dạng lưới và mỗi bước di chuyển có cùng trọng số.

### Tối ưu lộ trình thu thập sách

Dự án có cơ chế tính toán lộ trình thu thập sách dựa trên:

* Ma trận khoảng cách giữa các điểm quan trọng.
* Khởi tạo lộ trình bằng chiến lược chèn điểm xa nhất.
* Cải thiện lộ trình bằng 2-opt.
* Lặp tìm kiếm cục bộ để giảm tổng số bước.

Cơ chế này giúp game có thể đặt giới hạn bước đi hợp lý và tăng yếu tố thử thách cho người chơi.

### Trie gợi ý tên sách

Mini-game rương sử dụng Trie để gợi ý tên sách từ các sách người chơi đang có trong túi đồ. Trie giúp tìm kiếm theo tiền tố nhanh và phù hợp với tính năng autocomplete.

## Một số điểm kỹ thuật

* Bản đồ được đọc từ JSON của Tiled.
* Tileset được cắt thành từng tile nhỏ để render.
* Xử lý flip/rotate tile bằng bit mask của Tiled.
* Kiểm tra va chạm với tường, sách, cửa và khu vực boss.
* Quản lý âm thanh nền và hiệu ứng âm thanh riêng.
* Sử dụng font chữ để render text trong menu, túi đồ và hướng dẫn.
* Tách logic thành nhiều class: bản đồ, nhân vật, menu, âm thanh, boss, rương đồ, thuật toán.

## Hạn chế hiện tại

* Dự án đang tập trung vào bản chạy local trên Windows.
* Chưa có release chính thức.
* Chưa có file hướng dẫn cài đặt chi tiết cho Linux/macOS.
* Một số tài nguyên như `.exe` và `.dll` đang nằm trực tiếp trong repository.
* Chưa có hệ thống lưu điểm hoặc lưu tiến trình chơi.

## Hướng phát triển

* Thêm màn hình hướng dẫn trong game.
* Thêm hệ thống lưu điểm / lưu tiến trình.
* Thêm nhiều map và nhiều loại enemy.
* Tối ưu cấu trúc thư mục source code.
* Tạo bản release `.zip` để người dùng tải và chạy dễ hơn.
* Thêm video demo, ảnh gameplay và mô tả rõ hơn về gameplay.
* Thêm CI build hoặc hướng dẫn build đa nền tảng.

## Tác giả

**Võ Văn Hiếu**

Repository: `VoVanHieu2006/PBL2`
