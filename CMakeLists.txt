cmake_minimum_required(VERSION 3.29)
project(WellSketch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/utility.cmake)

find_package(
  Qt6
  COMPONENTS Gui Widgets Linguist 
  REQUIRED)

add_subdirectory(LCore)
add_subdirectory(LCPlot)
add_subdirectory(LLogPlotViewer)
add_subdirectory(WellSketchViewer)




