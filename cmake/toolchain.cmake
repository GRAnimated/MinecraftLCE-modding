set(CMAKE_SYSTEM_NAME Generic-ELF)
set(CMAKE_SYSTEM_VERSION "Marshmallow")
set(CMAKE_SYSTEM_PROCESSOR "aarch64")

# for multiplatform builds to identify that they are building for the switch
set(SWITCH TRUE)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

if(WIN32)
  set(EXE_EXT ".exe")
endif()

get_filename_component(TOOLS_DIR "${CMAKE_CURRENT_LIST_DIR}/../tools" ABSOLUTE)
set(TOOLS_TMP_DIR "${TOOLS_DIR}/tmp")

if (NOT EXISTS "${TOOLS_DIR}/clang/bin/clang${EXE_EXT}")
  if (WIN32)
    set(CLANG_DOWNLOAD_URL "https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm/releases/download/release-17.0.1/LLVMEmbeddedToolchainForArm-17.0.1-Windows-x86_64.zip")
    set(CLANG_FOLDER_NAME "LLVMEmbeddedToolchainForArm-17.0.1-Windows-x86_64")
    set(CLANG_ARCHIVE_EXT "zip")
  elseif(LINUX)
    set(CLANG_DOWNLOAD_URL "https://github.com/ARM-software/LLVM-embedded-toolchain-for-Arm/releases/download/release-17.0.1/LLVMEmbeddedToolchainForArm-17.0.1-Linux-x86_64.tar.xz")
    set(CLANG_FOLDER_NAME "LLVMEmbeddedToolchainForArm-17.0.1-Linux-x86_64")
    set(CLANG_ARCHIVE_EXT "tar.xz")
  else()
    message(FATAL_ERROR "Unsupported platform! Only Windows and Linux are supported.")
  endif()

  message(STATUS "Downloading clang ${TOOLS_TMP_DIR}/clang.${CLANG_ARCHIVE_EXT}")
  if (NOT EXISTS "${TOOLS_TMP_DIR}/clang.${CLANG_ARCHIVE_EXT}")
    file(DOWNLOAD "${CLANG_DOWNLOAD_URL}" "${TOOLS_TMP_DIR}/clang.${CLANG_ARCHIVE_EXT}" SHOW_PROGRESS)
  endif()
  file(ARCHIVE_EXTRACT INPUT "${TOOLS_TMP_DIR}/clang.${CLANG_ARCHIVE_EXT}" DESTINATION "${TOOLS_TMP_DIR}")
  file(REMOVE_RECURSE "${TOOLS_DIR}/clang")
  file(RENAME "${TOOLS_TMP_DIR}/${CLANG_FOLDER_NAME}" "${TOOLS_DIR}/clang")

  set(CONFIG_SITE_SOURCE "${TOOLS_DIR}/patches/__config_site")
  set(CONFIG_SITE_DESTINATION "${TOOLS_DIR}/clang/lib/clang-runtimes/aarch64-none-elf/aarch64/include/c++/v1")

  if(EXISTS "${CONFIG_SITE_DESTINATION}")
    file(REMOVE "${CONFIG_SITE_DESTINATION}")
  endif()

  # Copy the __config_site file
  file(COPY "${CONFIG_SITE_SOURCE}" DESTINATION "${CONFIG_SITE_DESTINATION}")
endif()

if (NOT EXISTS "${TOOLS_DIR}/linkle${EXE_EXT}")
  if (WIN32)
    set(LINKLE_DOWNLOAD_URL "https://github.com/MegatonHammer/linkle/releases/download/v0.2.11/linkle-refs.tags.v0.2.11-x86_64-pc-windows-msvc.zip")
  elseif (LINUX)
    set(LINKLE_DOWNLOAD_URL "https://github.com/MegatonHammer/linkle/releases/download/v0.2.11/linkle-refs.tags.v0.2.11-x86_64-unknown-linux-musl.zip")
  else()
    message(FATAL_ERROR "Unsupported platform! Only Windows and Linux are supported.")
  endif()
  file(DOWNLOAD "${LINKLE_DOWNLOAD_URL}" "${TOOLS_TMP_DIR}/linkle.zip" SHOW_PROGRESS)
  file(ARCHIVE_EXTRACT INPUT "${TOOLS_TMP_DIR}/linkle.zip" DESTINATION "${TOOLS_DIR}")
  file(CHMOD ${TOOLS_DIR}/linkle${EXE_EXT} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif ()

if(NOT EXISTS "${TOOLS_DIR}/curl")
  if (WIN32)
    set(CURL_DOWNLOAD_URL "https://github.com/stunnel/static-curl/releases/download/8.6.0-1/curl-windows-x86_64-8.6.0.tar.xz")
  elseif (LINUX)
    set(CURL_DOWNLOAD_URL "https://github.com/stunnel/static-curl/releases/download/8.6.0-1/curl-linux-x86_64-8.6.0.tar.xz")
  else()
    message(FATAL_ERROR "Unsupported platform! Only Windows and Linux are supported.")
  endif()
  file(DOWNLOAD "${CURL_DOWNLOAD_URL}" "${TOOLS_TMP_DIR}/curl.tar.xz" SHOW_PROGRESS)
  file(MAKE_DIRECTORY "${TOOLS_DIR}/curl")
  file(ARCHIVE_EXTRACT INPUT "${TOOLS_TMP_DIR}/curl.tar.xz" DESTINATION "${TOOLS_DIR}/curl")

  file(CHMOD ${TOOLS_DIR}/curl${EXE_EXT} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif()

set(CMAKE_C_COMPILER "${TOOLS_DIR}/clang/bin/clang${EXE_EXT}")
set(CMAKE_CXX_COMPILER "${TOOLS_DIR}/clang/bin/clang++${EXE_EXT}")
set(CMAKE_ASM_COMPILER "${TOOLS_DIR}/clang/bin/clang${EXE_EXT}")
set(CMAKE_LINKER "${DEVKITA64}/clang/bin/ld.lld${EXE_EXT}")
set(CURL_LOCATION "${TOOLS_DIR}/curl/curl${EXE_EXT}")
file(REAL_PATH "${TOOLS_DIR}/clang/bin/clang++${EXE_EXT}" CLANGD_DRIVER_PATH)

# generate a .clangd file
file(WRITE "${CMAKE_CURRENT_LIST_DIR}/../.clangd" "CompileFlags:\n  Remove: [-mcpu=]\n  Compiler: ${CLANGD_DRIVER_PATH}")

# generate .vscode/settings.json
if (FTP_IP)
  set(CONFIGURE_FTP_IP "-DFTP_IP=${FTP_IP}")
endif()
if (FTP_PORT)
  set(CONFIGURE_FTP_PORT "-DFTP_PORT=${FTP_PORT}")
endif()
if (FTP_USERNAME)
  set(CONFIGURE_FTP_USERNAME "-DFTP_USERNAME=${FTP_USERNAME}")
endif()
if (FTP_PASSWORD)
  set(CONFIGURE_FTP_PASSWORD "-DFTP_PASSWORD=${FTP_PASSWORD}")
endif()
if (FTP_TITLE_ID)
  set(CONFIGURE_FTP_TITLE_ID "-DFTP_TITLE_ID=${FTP_TITLE_ID}")
endif()
configure_file("${CMAKE_CURRENT_LIST_DIR}/../.vscode/settings.template.jsonc" "${CMAKE_CURRENT_LIST_DIR}/../.vscode/settings.json" @ONLY)

set(CMAKE_C_COMPILER_WORKS true)
set(CMAKE_CXX_COMPILER_WORKS true)
set(CMAKE_ASM_COMPILER_WORKS true)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp -g")
