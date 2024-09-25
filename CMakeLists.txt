cmake_minimum_required(VERSION 3.21)
project(marshmallow CXX C ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/user/src/*.cpp ${PROJECT_SOURCE_DIR}/user/src/*.c ${PROJECT_SOURCE_DIR}/user/src/*.hpp ${PROJECT_SOURCE_DIR}/user/src/*.h)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-fcolor-diagnostics)

add_compile_options(--target=aarch64-none-elf)
add_compile_options(-mcpu=cortex-a57+fp+simd+crypto+crc)
add_compile_options(-mno-implicit-float)
add_compile_options(-stdlib=libc++)
add_compile_options(-fPIC)
add_compile_options(-fstandalone-debug)
add_compile_options(-O3 -g -nostdlib -ffunction-sections -fdata-sections)
add_compile_options(-fno-rtti -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -fvisibility=default)
add_compile_options(-stdlib=libc++)
add_compile_options(-Wall -Wno-unused-parameter)
add_compile_definitions(ARDUINOJSON_ENABLE_COMMENTS SWITCH)
add_link_options(-nostdlib)
add_link_options(-fPIC -Wl,-Bsymbolic-functions -shared)
add_link_options(-fuse-ld=lld)

include(cmake/Helpers.cmake)

# Change this depending on the SDK version you are targeting
set(NN_SDK_MAJOR 4)
set(NN_SDK_MINOR 4)
set(NN_SDK_PATCH 1)
set(NN_SDK_TYPE "Release")

add_subdirectory(libs/nnsdk)
add_subdirectory(libs/exlaunch)
add_subdirectory(libs/ArduinoJson)
add_subdirectory(libs/marshmallow)
target_include_directories(marshmallow
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/user/src

        ${CMAKE_CURRENT_SOURCE_DIR}/libs/minecraft/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/minecraft/src
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/clang/lib/clang-runtimes/aarch64-none-elf/aarch64/include/c++/v1
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/clang/lib/clang-runtimes/aarch64-none-elf/aarch64/include
)

add_executable(subsdk9 ${SOURCES})

target_link_libraries(subsdk9 PUBLIC marshmallow
        -T ${CMAKE_SOURCE_DIR}/cmake/linker.ld
        -T ${CMAKE_SOURCE_DIR}/user/syms.txt
        -Wl,--whole-archive
        -Wl,--export-dynamic
        -Wl,--hash-style=sysv
        -Wl,--build-id=uuid
        -Wl,-init,rtldInit
)
generate_npdm(subsdk9 ${CMAKE_SOURCE_DIR}/user/subsdk9.json)
make_nso_target(subsdk9)
set(FTP_TITLE_ID 0100000000010000 CACHE STRING "FTP Title ID")
deploy_nso_ftp(subsdk9 ${FTP_TITLE_ID})
