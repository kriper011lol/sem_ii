@echo off
setlocal enabledelayedexpansion

:: ==========================================
:: НАСТРОЙКИ ПУТЕЙ
:: ==========================================
set QT_BIN=C:\msys64\ucrt64\bin
set QT_TOOLS=C:\msys64\ucrt64\share\qt6\bin
set CMAKE_EXE=cmake
set NINJA_EXE=ninja

:: Добавляем пути в начало PATH
set PATH=%QT_BIN%;%QT_TOOLS%;%PATH%

:: ==========================================
:: ЭТАП 1: Подготовка папок
:: ==========================================
set BUILD_DIR=build-release
set DIST_DIR=dist

echo [INFO] Очистка старых сборок...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
if exist %DIST_DIR% rmdir /s /q %DIST_DIR%

mkdir %BUILD_DIR%
mkdir %DIST_DIR%

:: ==========================================
:: ЭТАП 2: Конфигурация и сборка проекта
:: ==========================================
echo [INFO] Запуск CMake конфигурации (Режим Release)...
"%CMAKE_EXE%" -B %BUILD_DIR% -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo [ERROR] Ошибка конфигурации CMake!
    exit /b %errorlevel%
)

echo [INFO] Запуск компиляции...
"%CMAKE_EXE%" --build %BUILD_DIR% --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Ошибка сборки!
    exit /b %errorlevel%
)

:: ==========================================
:: ЭТАП 3: Деплой зависимостей
:: ==========================================
echo [INFO] Копирование исполняемого файла в папку %DIST_DIR%...
copy "%BUILD_DIR%\ComplexCalculator.exe" "%DIST_DIR%\"

echo [INFO] Запуск windeployqt6...
"%QT_BIN%\windeployqt6.exe" --release --compiler-runtime --no-translations --dir "%DIST_DIR%" "%DIST_DIR%\ComplexCalculator.exe"

echo [INFO] Копирование недостающих библиотек рантайма GCC/MinGW и зависимостей Qt6...
copy "%QT_BIN%\libgcc_s_seh-1.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libstdc++-6.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libwinpthread-1.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libdouble-conversion.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libpcre2-16-0.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libzstd.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libicudt*.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libicuin*.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libicuuc*.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libbrotlicommon.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libbrotlidec.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libpng16-16.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libharfbuzz-0.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libfreetype-6.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libglib-2.0-0.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libintl-8.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libiconv-2.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libgraphite2.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libbz2-1.dll" "%DIST_DIR%\"
copy "%QT_BIN%\zlib1.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libmd4c.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libb2-1.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libpcre2-8-0.dll" "%DIST_DIR%\"
copy "%QT_BIN%\libjpeg-8.dll" "%DIST_DIR%\"

echo [SUCCESS] Сборка и деплой успешно завершены! Все необходимые файлы находятся в папке "dist".
:: pause