cmake_minimum_required(VERSION 3.16)
project(MLTA_2.1)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Сборка для Windows (MinGW64)")
    set(PLATFORM "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Сборка для Linux")
    set(PLATFORM "linux")
else()
    message(STATUS "Сборка для неизвестной платформы: ${CMAKE_SYSTEM_NAME}")
    set(PLATFORM "unknown")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Настройки для Qt6
if(PLATFORM STREQUAL "windows")
    set(CMAKE_PREFIX_PATH /usr/x86_64-w64-mingw32/sys-root/mingw)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    qt_standard_project_setup()
elseif(PLATFORM STREQUAL "linux")
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    qt_standard_project_setup()
else()
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    qt_standard_project_setup()
endif()

# Добавляем директории с исходниками и заголовками
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/gui)

set(SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/TableInputWidget.cpp
    src/gui/ResultsWidget.cpp
    src/core/TruthTable.cpp
    src/core/PostClasses.cpp
    src/core/FunctionSet.cpp
    src/core/CompletenessChecker.cpp
)

set(HEADERS
    src/gui/MainWindow.h
    src/gui/TableInputWidget.h
    src/gui/ResultsWidget.h
    src/core/TruthTable.h
    src/core/PostClasses.h
    src/core/FunctionSet.h
    src/core/CompletenessChecker.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

message(STATUS "========================================")
message(STATUS "Конфигурация сборки:")
message(STATUS "  Проект: ${PROJECT_NAME}")
message(STATUS "  Платформа: ${PLATFORM}")
message(STATUS "  Система: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Процессор: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  Компилятор C: ${CMAKE_C_COMPILER}")
message(STATUS "  Компилятор C++: ${CMAKE_CXX_COMPILER}")
message(STATUS "  Папка сборки: ${CMAKE_BINARY_DIR}")
message(STATUS "========================================")