add_library(Core STATIC
    src/GMath.cpp
    src/Mesh.cpp
    include/GMath.hpp
    include/Vertex.hpp
    include/Mesh.hpp
)

target_include_directories(Core PUBLIC include)

target_link_libraries(Core PRIVATE tinyobjloader)

add_library(Graphics::core ALIAS Core)