# Mô phỏng vi mô giao thông đô thị đa tác tử

Đồ án OOP: SFML 3.x, C++17, State / Observer / Composite, `std::thread` + `std::mutex`.

## Cấu trúc

| Thành phần | Vai trò |
|---|---|
| `IObserver` / `ISubject` | Observer: đèn (Subject) báo xe (Observer) khi đổi màu |
| `GreenState` / `YellowState` / `RedState` | State: vòng đời đèn xanh → vàng → đỏ |
| `RoadNetwork` / `Lane` / `Intersection` | Composite: mạng đường chứa làn và nút giao |
| `Vehicle` (`Car`, `Motorcycle`) | Tác tử: mỗi xe một luồng, bám xe trước, phanh đèn đỏ |
| `Simulation` | Vòng lặp SFML, spawn xe, vẽ |

## Build (Windows - MSYS2 UCRT64)

Cài [MSYS2](https://www.msys2.org/), mở terminal **UCRT64**, sau đó cài compiler,
CMake và SFML:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-cmake \
  mingw-w64-ucrt-x86_64-sfml
```

Từ thư mục gốc của dự án:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --config Release
```

Chạy:

```bash
./build/urban_traffic.exe
```

## Chạy bằng VS Code

Mở thư mục dự án trong VS Code và cài extension **C/C++** của Microsoft.
Nhấn `F5`, sau đó chọn cấu hình `Run Urban Traffic`. VS Code sẽ tự configure,
build và chạy chương trình.

## Linux

```bash
sudo apt install libsfml-dev cmake g++
cmake -B build && cmake --build build
./build/urban_traffic
```

ESC hoặc đóng cửa sổ để thoát.
