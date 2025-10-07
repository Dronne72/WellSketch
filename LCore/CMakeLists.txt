project(LCore LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

GLOB_SOURCES()

set(COMMON_PATH ../common CACHE PATH "")

add_library(${PROJECT_NAME} STATIC ${H_FILES} ${CPP_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	${COMMON_PATH}
)
