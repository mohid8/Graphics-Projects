cmake_minimum_required(VERSION 3.22)
project(GraphicsProjects CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/include"
)

if(MSVC)
    add_compile_options(/W4 /permissive- /Oi)
else()
    add_compile_options(-Wall -Wextra -Werror -O3)
endif()

add_subdirectory(Core)
add_subdirectory(CPURasterizer)
# add_subdirectory(CPUPathTracer)       # Will uncomment later
# add_subdirectory(Engine)              # Will uncomment later