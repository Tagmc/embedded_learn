cmake_minimum_required(VERSION 3.20)

project(TaskManagerApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_THREAD_SANITIZER "Enable thread sanitizer" OFF)

if (ENABLE_THREAD_SANITIZER)
    add_compile_options(
        -fsanitize=thread
        -fno-omit-frame-pointer
    )

    add_link_options(
        -fsanitize=thread
    )
endif()

if(ENABLE_SANITIZERS)
    add_compile_options(
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )

    add_link_options(
        -fsanitize=address,undefined
    )
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)

    if(CLANG_TIDY_EXE)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
    else()
        message(WARNING "clang-tidy not found")
    endif()
endif()

function(enable_warnings target_name)
    target_compile_options(${target_name}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endfunction()

add_library(app_core
    src/Task.cpp
    src/TaskManager.cpp
)

target_include_directories(app_core
    PUBLIC
        include
)

enable_warnings(app_core)

add_executable(task_manager
    main.cpp
)

target_link_libraries(task_manager
    PRIVATE
        app_core
)

enable_warnings(task_manager)




if(BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(task_manager_tests
        tests/TaskManagerTest.cpp
    )

    target_link_libraries(task_manager_tests
        PRIVATE
            app_core
            GTest::gtest_main
    )

    enable_warnings(task_manager_tests)

    include(GoogleTest)

    gtest_discover_tests(task_manager_tests)
endif()