#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint vbo[2];

GLchar *vertexsource, *fragmentsource; /* These pointers will receive the contents of our shader source code files */
GLuint vertexshader, fragmentshader; /* These are handles used to reference the shaders */
GLuint shaderprogram; /* This is a handle to the shader program */

char* filetobuf(char *file)
{ /* A simple function that will read a file into an allocated char pointer buffer */
    FILE *fptr;
    long length;
    char *buf;
    fprintf(stderr, "Loading %s\n", file);
    fptr = fopen(file, "rb"); /* Open file for reading */

    if (!fptr) { /* Return NULL on failure */
            fprintf(stderr, "failed to open %s\n", file);
            return NULL;
            }

    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
    return buf; /* Return the buffer */
}

void setupShaders()
{

    vertexsource = filetobuf("shader.vert");
    fragmentsource = filetobuf("shader.frag");

    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexshader, 1, (const GLchar**) &vertexsource, 0);
    glShaderSource(fragmentshader, 1, (const GLchar**) &fragmentsource, 0);

    glCompileShader(vertexshader);
    glCompileShader(fragmentshader);

    shaderprogram = glCreateProgram();

    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);

    glLinkProgram(shaderprogram);
    glUseProgram(shaderprogram);

}

void setupGeometry(GLfloat **poly, GLfloat **colors)
{
    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), poly, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_LINE_LOOP, 0, 8);
    glFlush();
}

int main(int argc, char** argv)
{
    /*(0,    ±1,   ±phi)
      (±1,   ±phi, 0)
      (±phi, 0,    ±1)*/

    GLfloat vertices[4][2] = {
        {-0.5, 0.5},
        {0.5, 0.5},
        {0.5, -0.5},
        {-0.5, -0.5}
    };
    GLfloat colours[4][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {1.0, 1.0, 1.0}
    };

    //sourced http://www.glfw.org/docs/latest/quick.html
    if(!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    GLFWwindow* window = glfwCreateWindow(1024, 720, "Title", NULL, NULL);

    //check if window is created
    if (!window)
    {
        std::cout << "GLFW failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    int error = glewInit();
    if(error != GLEW_OK)
    {
        std::cout << "glewInit failed";
        exit(EXIT_FAILURE);
    }

    setupGeometry((GLfloat**)vertices, (GLfloat**)colours);
    setupShaders();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    while(!glfwWindowShouldClose(window))
    {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

}


