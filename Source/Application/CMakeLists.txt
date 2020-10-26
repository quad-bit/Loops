
#get all the source files
file(GLOB_RECURSE sourceList
    "*.h"
    "*.cpp"
)

#create library
add_Executable(Application
${sourceList})

# forces anyone who uses this lib to include the below mentioned source dir into their project props
#target_include_directories(Application PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(Application PUBLIC Core)
target_link_libraries(Application PUBLIC Graphics)
