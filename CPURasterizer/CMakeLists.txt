add_executable(CPURasterizer 
    main.cpp
    Rasterizer.h
    Rasterizer.cpp
)

target_link_libraries(CPURasterizer PRIVATE Core)

target_link_libraries(CPURasterizer PRIVATE
    glfw
    opengl32.lib
    user32.lib
    gdi32.lib
    shell32.lib
)