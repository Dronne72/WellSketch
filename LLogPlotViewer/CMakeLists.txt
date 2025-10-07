project(LLogPlotViewer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

GLOB_SOURCES()

set(COMMON_PATH ../common CACHE PATH "")
set(Q_CUSTOM_PLOT_PATH ../LCPlot)


add_definitions( -DQCUSTOMPLOT_USE_OPENGL )

add_library(${PROJECT_NAME} STATIC ${H_FILES} ${CPP_FILES} ${UI_FILES})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets PrintSupport LinguistTools  )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets PrintSupport LinguistTools  )

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Qt${QT_VERSION_MAJOR}::Core 
	Qt${QT_VERSION_MAJOR}::Widgets 
	Qt${QT_VERSION_MAJOR}::PrintSupport
	Qt${QT_VERSION_MAJOR}::OpenGL
	Qt${QT_VERSION_MAJOR}::OpenGLWidgets
	#Qt${QT_VERSION_MAJOR}::Linguist  
	LCPlot
	opengl32)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	${Q_CUSTOM_PLOT_PATH}
)
