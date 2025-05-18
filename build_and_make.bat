@echo off
setlocal

:: ✅ 建立 build 資料夾（若已存在則略過）
if not exist build (
    mkdir build
)

:: ✅ 進入 build 資料夾
cd build

:: ✅ 執行 CMake 配置
cmake -G "MinGW Makefiles" ..

:: ✅ 編譯專案
mingw32-make

endlocal
pause
