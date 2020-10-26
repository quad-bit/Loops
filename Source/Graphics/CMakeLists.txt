
##  file list creation Begin

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist "${child}")
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/Rendering")

Set ( SourceList "")

FOREACH(subdir ${SUBDIRS})
    #message(${subdir})

    file(GLOB fileListInDir
        "Rendering/${subdir}/*.h"
        "Rendering/${subdir}/*.cpp"
        "Rendering/${subdir}/*.txt"
    )
    LIST(APPEND SourceList ${fileListInDir})

    source_group(${subdir} FILES ${fileListInDir})

ENDFOREACH()

message("\n")

#get all the source files
file(GLOB filesInParentDir
    "Rendering/*.h"
    "Rendering/*.cpp"
    "Rendering/*.txt"
)

LIST(APPEND SourceList ${filesInParentDir})

##  file list creation ends
#create library
add_library(Renderer
${SourceList} )

FOREACH(subdir ${SUBDIRS})
    # forces anyone who uses this lib to include the below mentioned source dir into their project props
    target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Rendering/${subdir}")

ENDFOREACH()

target_link_libraries(Renderer PUBLIC Core)

IF( APPLE )
  MESSAGE( STATUS "Running under MacOS X" )
# Watch out, for this check also is TRUE under MacOS X because it
# falls under the category of Unix-like.
ELSEIF( UNIX )
  MESSAGE( STATUS "Running under Unix or a Unix-like OS" )
# Despite what you might think given this name, the variable is also
# true for 64bit versions of Windows.
ELSEIF( WIN32 )
  MESSAGE( STATUS "Running under Windows (either 32bit or 64bit), setting character set to unicode" )
  add_definitions(-DUNICODE -D_UNICODE)


ENDIF()
