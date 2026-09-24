# Urban Traffic Simulation

Dự án mô phỏng giao thông đô thị bằng C++ + SFML.

## Chạy nhanh trên Windows

### 1) Cài đặt toolchain cần thiết
Cài đặt MSYS2 rồi mở terminal MSYS2 UCRT64 và chạy:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-sfml
```

### 2) Mở project trong VS Code
Mở thư mục dự án trong VS Code.

### 3) Chạy bằng F5
Nhấn `F5`, rồi chọn cấu hình:

```text
Run Urban Traffic
```

VS Code sẽ build và mở chương trình.

## Nếu muốn chạy thủ công
Trong terminal PowerShell hoặc terminal của VS Code, chạy:

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
g++.exe -std=c++17 -Iinclude src/*.cpp -o urban_traffic.exe -lsfml-graphics -lsfml-window -lsfml-system
.\urban_traffic.exe
```

## Tính năng chính
- Đèn giao thông có trạng thái xanh - vàng - đỏ
- Xe di chuyển theo làn, tránh xe phía trước
- Xe dừng khi đèn đỏ hoặc vàng gần vạch dừng
- Mô phỏng dạng đơn giản nhưng theo hướng OOP

## Thoát chương trình
- Nhấn `ESC`
- Hoặc đóng cửa sổ

## Ghi chú
Dự án này dùng SFML và compiler MinGW/MSYS2 trên Windows. Vì vậy, nếu máy chưa có MSYS2/UCRT64 và SFML thì project sẽ không chạy được.
