#include <GLFW/glfw3.h>
#include <iostream>
#include "Rasterizer.h"
#include <intrin.h>

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
    GMath::Vertex v0;
    GMath::Vertex v1;
    GMath::Vertex v2;

    v0.pos = GMath::Vec4(10,10,0,0);
    v1.pos = GMath::Vec4(80,80,0,0);
    v2.pos = GMath::Vec4(80,10,0,0);

    v0.color = GMath::Vec4(1,0,0,1);
    v1.color = GMath::Vec4(0,1,0,1);
    v2.color = GMath::Vec4(0,0,1,1);

    uint64_t minNumCycles = 0xFFFFFFFFFFFFFFFF;
    uint64_t maxNumCycles = 0x0000000000000000;
    uint64_t avgCycles = 0;
    int numIter = 1000;
    for(int i = 0; i<numIter; i++)
    {
        _mm_lfence();
        uint64_t startCycles = __rdtsc();
        _mm_lfence();

        renderer.drawTriangle(v0, v1, v2);

        _mm_lfence();
        uint64_t endCycles = __rdtsc();
        _mm_lfence();

        uint64_t totalCycles = endCycles - startCycles;
        avgCycles += totalCycles;
        minNumCycles = std::min(minNumCycles, totalCycles);
        maxNumCycles = std::max(maxNumCycles, totalCycles);         
    }
    avgCycles /= numIter;
    std::cout << "Num of Function Calls: " << numIter << "\n";
    std::cout << "Minimum Cycles: " << minNumCycles << "\n";
    std::cout << "Maximum Cycles: " << maxNumCycles << "\n";
    std::cout << "Average Cycles: " << avgCycles << "\n";

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