cmake_minimum_required(VERSION 3.16)

# Название проекта и используемый язык
project(ComplexCalculator VERSION 1.0.0 LANGUAGES CXX)

# Задаем стандарт C++17 (Qt6 требует минимум C++17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
# Отключаем специфичные расширения компилятора (например, gnu++17), используем чистый стандарт
set(CMAKE_CXX_EXTENSIONS OFF)

# Включаем автоматическую генерацию мета-кода для Qt (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Ищем компоненты Qt6
find_package(Qt6 COMPONENTS Widgets Gui GuiPrivate REQUIRED)
find_package(QXlsxQt6 REQUIRED)

add_subdirectory(minidocx)

# Исходные файлы разбиты на ядро и GUI
set(CORE_SOURCES
    core.h
    core.cpp
)

set(GUI_SOURCES
    mainwindow.h
    mainwindow.cpp
    main.cpp
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${CORE_SOURCES} ${GUI_SOURCES})

# Прячем консоль Windows в релизной сборке. 
if(WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE ON)
endif()

# Линкуем библиотеку Qt6::Widgets к нашему проекту
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets QXlsx::QXlsx minidocx)
target_include_directories(${PROJECT_NAME} PRIVATE minidocx/include/minidocx)

# Русские строки в исходниках должны одинаково читаться GCC/Clang/MSVC на Windows, Linux и macOS.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
