cmake_minimum_required(VERSION 3.15)

include(${CMAKE_CURRENT_LIST_DIR}/Cmake/vcpkg.cmake)

project (Loops)

message ("using c++ 20")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message ("\nConfiguring Core ..\n")
add_subdirectory(Source/Core Projects/Core)

message ("\nConfiguring Vulkan Driver ..\n")
add_subdirectory(Source/Graphics/Drivers/Vulkan Projects/Vulkan)

message ("\nGraphics ..\n")
add_subdirectory(Source/Graphics Projects/Graphics)

message ("\nConfiguring EngineInterface ..\n")
add_subdirectory(Source/EngineInterface Projects/EngineInterface)

message ("\nApp ..")
add_subdirectory(Source/Application Projects/Application)
