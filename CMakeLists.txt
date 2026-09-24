cmake_minimum_required(VERSION 3.16)
project(UrbanTrafficSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3.x (MSYS2: pacman -S mingw-w64-ucrt-x86_64-sfml mingw-w64-ucrt-x86_64-cmake)
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(urban_traffic
    src/main.cpp
    src/TrafficLightState.cpp
    src/TrafficLight.cpp
    src/Lane.cpp
    src/Intersection.cpp
    src/RoadNetwork.cpp
    src/Vehicle.cpp
    src/Simulation.cpp
)

target_include_directories(urban_traffic PRIVATE include)
target_link_libraries(urban_traffic PRIVATE SFML::Graphics)

if(MSVC)
    target_compile_options(urban_traffic PRIVATE /utf-8 /W4)
else()
    target_compile_options(urban_traffic PRIVATE -Wall -Wextra)
endif()

if(WIN32)
    add_custom_command(TARGET urban_traffic POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics>
            $<TARGET_FILE:SFML::Window>
            $<TARGET_FILE:SFML::System>
            $<TARGET_FILE_DIR:urban_traffic>
    )
endif()
