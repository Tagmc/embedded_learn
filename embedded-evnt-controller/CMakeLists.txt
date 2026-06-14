cmake_minimum_required(VERSION 3.20)

project(EmbeddedEventController LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(embedded_core
    src/Register32.cpp
    src/GpioPort.cpp
    src/GpioPin.cpp
    src/Led.cpp
    src/Button.cpp
    src/SimpleController.cpp
    src/VehicleController.cpp
    src/CommandParser.cpp
)

target_include_directories(embedded_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(embedded_core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(embedded_event_controller
    app/main.cpp
)


target_link_libraries(embedded_core
    PUBLIC
        Threads::Threads
)

target_link_libraries(embedded_event_controller
    PRIVATE
        embedded_core
        Threads::Threads
)

target_compile_options(embedded_event_controller
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)