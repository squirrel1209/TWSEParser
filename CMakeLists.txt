cmake_minimum_required(VERSION 3.14)
project(TWSEPacketParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =======================================
# 🔧 Include headers（已刪除 base）
# =======================================
include_directories(
    include
    include/common
    include/format1
    include/format6
    include/result
    include/controller
)

# =======================================
# 📦 核心 Source files（已刪除 TWSEPacketBase）
# =======================================
set(CORE_SOURCES
    src/main.cpp
    src/controller/TWSEApp.cpp
    src/controller/TWSEPacketParser.cpp
    src/storage/TWSEDatabase.cpp
    src/base/TWSEParserFactory.cpp
    src/common/EncodingUtils.cpp
    src/format1/TWSEFormat1.cpp
    src/format1/TWSEFormat1Parser.cpp
    src/format6/TWSEFormat6.cpp
    src/format6/TWSEFormat6Parser.cpp
)

# =======================================
# 🎯 Main Executable
# =======================================
add_executable(TWSEParser ${CORE_SOURCES})

# =======================================
# 🧪 GoogleTest (內建 FetchContent)
# =======================================
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# =======================================
# 🧪 Test Source files
# =======================================
set(TEST_SOURCES
    test/test_format1.cpp
    # test/test_format6.cpp
    # test/test_parser_factory.cpp
)

# ✅ 單元測試執行檔
add_executable(TWSETests ${TEST_SOURCES})

# ✅ 測試需要的核心元件
target_sources(TWSETests
    PRIVATE
    src/format1/TWSEFormat1.cpp
    src/format1/TWSEFormat1Parser.cpp
    src/format6/TWSEFormat6.cpp
    src/format6/TWSEFormat6Parser.cpp
    src/controller/TWSEPacketParser.cpp
    src/base/TWSEParserFactory.cpp
    src/common/EncodingUtils.cpp
)

# ✅ 連結 GoogleTest
target_link_libraries(TWSETests gtest gtest_main)

# ✅ 測試註冊
add_test(NAME AllTests COMMAND TWSETests)

# =======================================
# ✅ 結尾提示
# =======================================
message(STATUS "🛠  TWSEPacketParser Project Ready!")
