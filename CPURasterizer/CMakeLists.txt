add_executable(CPURasterizer 
    main.cpp
    Rasterizer.h
    Pipeline.h
    Rasterizer.cpp    
    Pipeline.cpp
)

target_link_libraries(CPURasterizer PRIVATE Graphics::core)

target_link_libraries(CPURasterizer PRIVATE
    glfw
    opengl32.lib
    user32.lib
    gdi32.lib
    shell32.lib
)