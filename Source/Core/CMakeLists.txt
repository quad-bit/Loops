
##  file list creation Begin

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

Set ( SourceList "")

FOREACH(subdir ${SUBDIRS})
    message(${subdir})

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

LIST(APPEND SourceList ${filesInParentDir})

##  file list creation ends

IF( glm_FOUND )
  message (" found glm ")
ELSE()
  SET( glm_DIR "$ENV{GLM_PATH}" )
  SET ( glm_INCLUDE_DIR "${glm_DIR}")
  message (" glm include directory set to : ${glm_INCLUDE_DIR}")
ENDIF()


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
add_library(Core
${SourceList})

# forces anyone who uses this lib to include the below mentioned source dir into their project props
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Core PUBLIC ${glm_INCLUDE_DIR})
target_include_directories(Core PUBLIC ${glfw3_INCLUDE_DIR})
