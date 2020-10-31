
# Gives all the subdirectories recursively
MACRO(HEADER_DIRECTORIES return_list curDir)

    get_filename_component(ABSOLUTE_PATH ${curDir} ABSOLUTE)
    #message(${curDir})

    FILE(GLOB_RECURSE new_list "${curDir}/*.h" "${curDir}/*.cpp" "${curDir}/*.txt" )
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} DIRECTORY)
        file(RELATIVE_PATH dir_path ${CMAKE_CURRENT_SOURCE_DIR} ${dir_path})
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()

    list(LENGTH dir_list size)
    IF(NOT ${size} EQUAL 0)
        LIST(REMOVE_DUPLICATES dir_list)
    ENDIF()

    SET(${return_list} ${dir_list})
ENDMACRO()

# Gives all the subdirectories (one level)... actually not that effective.
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    message(${child})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()

  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

MACRO(ADD_GLFW includeDir libDir libraries)
    message("Adding GLFW")
    #find glfw
    IF( DEFINED ENV{GLFW_PATH} )
      SET( glfw_DIR "$ENV{GLFW_PATH}" )
    ENDIF()

    #find_package(glfw HINTS "${glfw_DIR}")
    find_package(glfw3 3.3)

    IF( glfw3_FOUND )
      message (" found glfw ")
    ELSE()
      message ("glfw package not found, adding it manually")
      SET ( glfw3_INCLUDE_DIR "${glfw_DIR}/include")
      SET ( glfw3_LIBRARY_DIR "${glfw_DIR}/Build/src/Debug")
      SET ( glfw3_LIBRARIES "${glfw3_LIBRARY_DIR}/glfw3.lib")
    ENDIF()

    SET ( includeDir ${glfw3_INCLUDE_DIR})
    SET ( libDir ${glfw3_LIBRARY_DIR} )
    SET ( libraries ${glfw3_LIBRARIES} )

ENDMACRO()
