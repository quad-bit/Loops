cmake_minimum_required ( VERSION 2.6)
project (Loops)

message ("using c++ 17")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message ("Configuring Core ..\n")
add_subdirectory(Source/Core Projects/Core)

message ("\nConfiguring Vulkan Driver ..\n")
add_subdirectory(Source/Graphics/Drivers/Vulkan Projects/Vulkan)

message ("Graphics ..")
add_subdirectory(Source/Graphics Projects/Graphics)

message ("App ..")
add_subdirectory(Source/Application Projects/Application)
