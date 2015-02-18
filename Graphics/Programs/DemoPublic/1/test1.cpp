#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include <utils.h>



int main( void ) {
    int k = 0;
// Initialize GLFW
    if( !glfwInit() ) {
            exit( EXIT_FAILURE );
            }
    glewInit();
    // Open an OpenGL window
    GLFWwindow* window;
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
            glfwTerminate();
            return -1;
            }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Main loop
    while( !glfwWindowShouldClose(window) ) {
            // OpenGL rendering goes here...
            if(k <= 70)
                glClearColor(1., 1., 0., 1.);
            if(k > 70)
                glClearColor(0., 1., 1., 1.);
            glClear( GL_COLOR_BUFFER_BIT );
            // Swap front and back rendering buffers
            glfwSwapBuffers(window);

            //Poll for and process events
            glfwPollEvents();
            k = k + 1;
            if(k > 140)
                k = 0;
            }
    // Close window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    // Exit program
    exit( EXIT_SUCCESS );
    }
