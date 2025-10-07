cmake_minimum_required(VERSION 3.16)  # Понижаем версию для лучшей совместимости
project(WellSketch VERSION 1.0.0 LANGUAGES CXX)

# Базовые настройки
set(CMAKE_CXX_STANDARD 17)  # Меняем на 17 для лучшей совместимости с Qt
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматизацию Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Устанавливаем путь к Qt6
set(QT_DIR "C:/Qt/6.8.2" CACHE PATH "Path to Qt6 installation")
if(EXISTS "${QT_DIR}")
    set(CMAKE_PREFIX_PATH "${QT_DIR}")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

# Проверяем найденные компоненты
if(Qt6_FOUND)
    message(STATUS "Qt6 version: ${Qt6_VERSION}")
    message(STATUS "Qt6 directories: ${Qt6_DIR}")
    message(STATUS "Qt6 components found: Core=${Qt6Core_FOUND}, Gui=${Qt6Gui_FOUND}, Widgets=${Qt6Widgets_FOUND}")
else()
    message("Qt6 not found! Please set QT_DIR variable")
endif()

# Включаем utility.cmake если он существует
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/utility.cmake")
    include(${CMAKE_SOURCE_DIR}/cmake/utility.cmake)
endif()

# Добавляем поддиректории с проверкой
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LCore")
    add_subdirectory(LCore)
else()
    message(WARNING "LCore directory not found")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LCPlot")
    add_subdirectory(LCPlot)
else()
    message(WARNING "LCPlot directory not found")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/LLogPlotViewer")
    add_subdirectory(LLogPlotViewer)
else()
    message(WARNING "LLogPlotViewer directory not found")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/WellSketchViewer")
    add_subdirectory(WellSketchViewer)
else()
    message(WARNING "WellSketchViewer directory not found")
endif()

# Настройка для установки
if(WIN32)
    # Копирование DLL после сборки
    add_custom_target(copy_dlls ALL)
endif()



