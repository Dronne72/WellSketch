project(LCPot LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

GLOB_SOURCES()

add_definitions( -DQCUSTOMPLOT_USE_OPENGL )

add_library(${PROJECT_NAME} STATIC ${H_FILES} ${CPP_FILES} ${UI_FILES}
	qcustomplot.cpp
	qcustomplot.h
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets PrintSupport)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt::Core 
	Qt::Widgets
	Qt::Linguist
	Qt6::PrintSupport
	Qt6::OpenGL
	Qt6::OpenGLWidgets
	opengl32
)
