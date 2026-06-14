#include <GLFW/glfw3.h>
#include <iostream>
#include "Rasterizer.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

const int RENDER_WIDTH = 100;
const int RENDER_HEIGHT = 100;

int main() 
{
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CPURasterizer", nullptr, nullptr);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    Rasterizer renderer(RENDER_WIDTH, RENDER_HEIGHT);
    renderer.fillColor(0xFFFFFFFF);
    renderer.drawLine(50,50,80,80,0xFFFF0000);

    float scaleX = (float)WINDOW_WIDTH / RENDER_WIDTH;
    float scaleY = (float)WINDOW_HEIGHT / RENDER_HEIGHT;

    glfwMakeContextCurrent(window);
    glPixelZoom(scaleX, -scaleY); /*flipping Y-axis since I want to draw top to bottom*/

    while (!glfwWindowShouldClose(window)) 
    {
        glRasterPos2f(-1.0f, 1.0f);
        
        glDrawPixels(RENDER_WIDTH, RENDER_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, renderer.getColorBuffer());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}