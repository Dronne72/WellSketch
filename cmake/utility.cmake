# Remove strings matching given regular expression from a list.
# @param(in,out) aItems Reference of a list variable to filter.
# @param aRegEx Value of regular expression to match.
# taken from https://stackoverflow.com/questions/24491129/excluding-directory-somewhere-in-file-structure-from-cmake-sourcefile-list
function (filter_items aItems aRegEx)
    # For each item in our list
    foreach (item ${${aItems}})
        # Check if our items matches our regular expression
        if ("${item}" MATCHES ${aRegEx})
            # Remove current item from our list
            list (REMOVE_ITEM ${aItems} ${item})
        endif ("${item}" MATCHES ${aRegEx})
    endforeach(item)
    # Provide output parameter
    set(${aItems} ${${aItems}} PARENT_SCOPE)
endfunction (filter_items)

#[[ GLOB_SOURCES macro forms the file list for the file extension and general source file list ]]#
macro(GLOB_SOURCES)
    set(SOURCES "")
    GLOB_SOURCES_GROUP("Source files" cpp)
    GLOB_SOURCES_GROUP("Header files" h)
    GLOB_SOURCES_GROUP("Ui files" ui)
    GLOB_SOURCES_GROUP("Qt resource files" qrc)
    GLOB_SOURCES_GROUP("Resource files" rc)
    GLOB_SOURCES_GROUP("Translate files" ts)
    GLOB_SOURCES_GROUP("Qml files" qml)
endmacro()

#[[ GLOB_SOURCES_GROUP macro forms the file list for the TYPE_NAME extension
and adds the list generated to the general source list
]]#
macro(GLOB_SOURCES_GROUP GROUP_NAME TYPE_NAME)
    string(TOUPPER ${TYPE_NAME} TYPE_NAME_UPPER)
    set(VARIABLE_NAME ${TYPE_NAME_UPPER}_FILES)
    set(${VARIABLE_NAME} "")
    GLOB_SOURCES_GROUP_FROM_DIR(${VARIABLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${TYPE_NAME})
    source_group(${GROUP_NAME} FILES ${${VARIABLE_NAME}})
endmacro()

macro(GET_SUBDIR_LIST DIR_PATH RESULT_VARIABLE)
    file(GLOB ${RESULT_VARIABLE} RELATIVE ${DIR_PATH} ${DIR_PATH}/*)
    foreach(NAME ${${RESULT_VARIABLE}})
        if((NOT IS_DIRECTORY ${DIR_PATH}/${NAME}) OR (${NAME} MATCHES "^\\."))
            list(REMOVE_ITEM ${RESULT_VARIABLE} ${NAME})
        endif()
    endforeach()
endmacro()

macro(GLOB_SOURCES_GROUP_FROM_DIR RESULT_VARIABLE_NAME DIR_PATH TYPE_NAME)
    file(GLOB FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${DIR_PATH}/*.${TYPE_NAME})
    list(APPEND ${RESULT_VARIABLE_NAME} ${FILES})
    GET_SUBDIR_LIST(${DIR_PATH} SUB_DIRS)
    foreach(DIR ${SUB_DIRS})
        set(ABSOLUTE_DIR_PATH ${DIR_PATH}/${DIR})
        GLOB_SOURCES_GROUP_FROM_DIR(${RESULT_VARIABLE_NAME} ${ABSOLUTE_DIR_PATH} ${TYPE_NAME})
    endforeach()
    list(APPEND SOURCES ${${RESULT_VARIABLE_NAME}})
endmacro()
