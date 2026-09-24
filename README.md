# Mô phỏng vi mô giao thông đô thị đa tác tử

Đồ án OOP: SFML 2.x, C++17, State / Observer / Composite, `std::thread` + `std::mutex`.

## Cấu trúc

| Thành phần | Vai trò |
|---|---|
| `IObserver` / `ISubject` | Observer: đèn (Subject) báo xe (Observer) khi đổi màu |
| `GreenState` / `YellowState` / `RedState` | State: vòng đời đèn xanh → vàng → đỏ |
| `RoadNetwork` / `Lane` / `Intersection` | Composite: mạng đường chứa làn và nút giao |
| `Vehicle` (`Car`, `Motorcycle`) | Tác tử: mỗi xe một luồng, bám xe trước, phanh đèn đỏ |
| `Simulation` | Vòng lặp SFML, spawn xe, vẽ |

## Build (Windows)

Cài [SFML 2.6](https://www.sfml-dev.org/download.php) hoặc vcpkg:

```bat
vcpkg install sfml:x64-windows
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[đường-dẫn-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

SFML giải nén thủ công:

```bat
cmake -B build -DSFML_DIR="C:/SFML/lib/cmake/SFML"
cmake --build build --config Release
```

Chạy `build/Release/urban_traffic.exe` (copy DLL SFML cùng thư mục nếu chưa tự copy).

## Linux

```bash
sudo apt install libsfml-dev cmake g++
cmake -B build && cmake --build build
./build/urban_traffic
```

ESC hoặc đóng cửa sổ để thoát.
