cmake_minimum_required(VERSION 3.16)

project(ComplexCalculator VERSION 1.0.0 LANGUAGES CXX)

# Режим сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Стандарт C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Автогенерация Qt файлов
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Поиск Qt6 (Core, Gui, Widgets)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Интеграция QXlsx через FetchContent
include(FetchContent)
FetchContent_Declare(
    QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG        master
    SOURCE_SUBDIR  QXlsx
)
FetchContent_MakeAvailable(QXlsx)

# Подключение minidocx
add_subdirectory(minidocx)

# Исходные файлы
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
add_executable(${PROJECT_NAME} WIN32 ${CORE_SOURCES} ${GUI_SOURCES})

# Линковка
# Примечание: QXlsx при использовании FetchContent обычно экспортирует цель QXlsx
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    QXlsx
    minidocx
)

# Пути к заголовкам minidocx
target_include_directories(${PROJECT_NAME} PRIVATE minidocx/include/minidocx)

# Поддержка UTF-8
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()