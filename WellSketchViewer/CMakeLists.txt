cmake_minimum_required(VERSION 3.16)

project(WellSketchViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        LinguistTools  
        OpenGL
        OpenGLWidgets
)

find_library(PROPERTY_EDITOR_LIB LCore)

set(LLOG_PLOT_VIEWER_PATH ../LLogPlotViewer CACHE PATH "")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ComponentsLibrary/componentlibrarywidget.cpp
        ComponentsLibrary/componentlibrarywidget.h
        ComponentsLibrary/headerwidget.cpp
        ComponentsLibrary/headerwidget.h
        ComponentsLibrary/icontextdelegate.cpp
        ComponentsLibrary/icontextdelegate.h
        ComponentsLibrary/componentsmodel.cpp
        ComponentsLibrary/componentsmodel.h
        ComponentsLibrary/componentstableview.cpp
        ComponentsLibrary/componentstableview.h
        WellSchematicTree/wellschematictree.cpp
        WellSchematicTree/wellschematictree.h
        # PropertiesWidget/propertieswidget.cpp
        # PropertiesWidget/propertieswidget.h
        PropertiesWidget/wellcomponentitem.h
        PropertiesWidget/wellcomponentitem.cpp
        WellSchematic/wellschematicviewwidget.cpp
        WellSchematic/wellschematicviewwidget.h
        # PropertiesWidget/propertiesdelegate.h
        # PropertiesWidget/propertiesdelegate.cpp
        WellSchematic/depthscalewidget.h
        WellSchematic/depthscalewidget.cpp
        WellSchematic/wellprofilewidget.h
        WellSchematic/wellprofilewidget.cpp
        WellSchematic/wellopenglwidget.h
        WellSchematic/wellopenglwidget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WellSketchViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
                                test_data.qrc
                                PropertiesWidget/propertiesview.h PropertiesWidget/propertiesview.cpp
                                ComponentsLibrary/logwidget.h ComponentsLibrary/logwidget.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WellSketchViewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(WellSketchViewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WellSketchViewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WellSketchViewer PRIVATE Qt::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Qt::Core 
	#Qt6::Widgets 
	Qt::PrintSupport
    Qt::Gui 
    Qt6::OpenGL
    Qt6::OpenGLWidgets

    LCore
    LCPlot
        LLogPlotViewer
    )



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WellSketchViewer)
endif()
set_target_properties(WellSketchViewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories (${PROJECT_NAME} PRIVATE
	 ${LLOG_PLOT_VIEWER_PATH}
)

include(GNUInstallDirs)
install(TARGETS WellSketchViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WellSketchViewer)
endif()
