@echo off

echo ============================================
echo STEP 1: CLEANING BUILD DIRECTORY
echo ============================================

if exist build (
    echo Deleting existing build folder...
    rmdir /S /Q build
) else (
    echo Build folder does not exist.
)

echo.
echo ============================================
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
echo ============================================

cmake -S . -B build -G Ninja
if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo STEP 3: COMPILING FIRMWARE WITH NINJA
echo ============================================

ninja -C build
if errorlevel 1 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
echo ============================================

STM32_Programmer_CLI ^
-c port=SWD ^
-w build\app_firmware.bin 0x08000000 ^
-Rst

if errorlevel 1 (
    echo.
    echo ERROR: Flash failed!
    pause
    exit /b 1
)

echo.
echo ============================================
echo Build and Flash completed successfully.
echo ============================================

pause