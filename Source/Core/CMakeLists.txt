
##  file list creation Begin

include("${CMAKE_CURRENT_SOURCE_DIR}/../../Util.cmake")

HEADER_DIRECTORIES(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

Set ( SourceList "")

FOREACH(subdir ${SUBDIRS})
    #message(${subdir})
    #file(RELATIVE_PATH test ${CMAKE_CURRENT_SOURCE_DIR} ${subdir})
    #message(${test})

    file(GLOB fileListInDir
        "${subdir}/*.h"
        "${subdir}/*.cpp"
        "${subdir}/*.txt"
    )
    LIST(APPEND SourceList ${fileListInDir})

    source_group(${subdir} FILES ${fileListInDir})

ENDFOREACH()

message("\n")

#get all the source files
file(GLOB filesInParentDir
    "*.h"
    "*.cpp"
    "*.txt"
)

list(LENGTH filesInParentDir size)
IF(NOT ${size} EQUAL 0)
    LIST(APPEND SourceList ${filesInParentDir})
    source_group("" FILES ${filesInParentDir})
    LIST(APPEND SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()

##  file list creation ends

find_package(glm REQUIRED)
IF( glm_FOUND )
  message (" found glm : ${glm_INCLUDE_DIR} ")
ELSE()
  SET( glm_DIR "$ENV{GLM_PATH}" )
  SET ( glm_INCLUDE_DIR "${glm_DIR}")
  message (" glm include directory set to : ${glm_INCLUDE_DIR}")
ENDIF()

#SET( plog_DIR "$ENV{GLM_PATH}" )
#SET ( plog_INCLUDE_DIR "${glm_INCLUDE_DIR}/../Logging/")
SET( plog_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/ThirdParty/Logging" )
message (" plog include directory set to : ${plog_INCLUDE_DIR}")


#find glfw
IF( DEFINED ENV{GLFW_PATH} )
  SET( glfw_DIR "$ENV{GLFW_PATH}" )
ENDIF()

#find_package(glfw HINTS "${glfw_DIR}")
find_package(glfw3 3.3)

IF( glfw3_FOUND )
  message (" found glfw ")
ELSE()
  message (" not found glfw ")
  SET ( glfw3_INCLUDE_DIR "${glfw_DIR}/include")
  SET ( glfw3_LIBRARY_DIR "${glfw_DIR}/Build/src/Debug")
  SET ( glfw3_LIBRARIES "${glfw3_LIBRARY_DIR}/glfw3.lib")
ENDIF()

#create library
add_library(Core ${SourceList})

target_link_libraries(Core PUBLIC glfw)
target_link_libraries(Core PRIVATE glm::glm)

# forces anyone who uses this lib to include the below mentioned source dir into their project props
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Core PUBLIC ${SUBDIRS})
# target_include_directories(Core PUBLIC ${glm_INCLUDE_DIR})
target_include_directories(Core PUBLIC ${plog_INCLUDE_DIR})
# target_include_directories(Core PUBLIC ${glfw3_INCLUDE_DIR})

if (MSVC)
    # For precompiled header.
    # Set
    # "Precompiled Header" to "Use (/Yu)"
    # "Precompiled Header File" to "stdafx.h"
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /YuCorePrecompiled.h /FICorePrecompiled.h")
endif()

set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/CorePrecompiled.cpp"
    PROPERTIES
    COMPILE_FLAGS "/YcCorePrecompiled.h"
)
