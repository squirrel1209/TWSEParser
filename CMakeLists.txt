cmake_minimum_required(VERSION 3.14)
project(TWSEPacketParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =======================================
# 🔧 Include headers
# =======================================
include_directories(
    include
    include/base
    include/common
    include/format1
    include/format6
    include/result
    include/controller
)

# =======================================
# 📦 Source files
# =======================================
file(GLOB_RECURSE SOURCES
    src/main.cpp
    src/controller/TWSEApp.cpp
    src/controller/TWSEPacketParser.cpp
    src/controller/TWSEDatabase.cpp
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
add_executable(TWSEParser ${SOURCES})

# =======================================
# 🧪 GoogleTest (built-in FetchContent)
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
# 🧪 Unit Test Executable
# =======================================
file(GLOB_RECURSE TEST_SOURCES
    test/test_format1.cpp
    # 你也可以加上其他：
    # test/test_format6.cpp
    # test/test_parser_factory.cpp
)

add_executable(TWSETests ${TEST_SOURCES})
target_link_libraries(TWSETests gtest gtest_main)

add_test(NAME AllTests COMMAND TWSETests)

# =======================================
# 🪪 結尾提示
# =======================================
message(STATUS "🛠  TWSEPacketParser Project Ready!")
