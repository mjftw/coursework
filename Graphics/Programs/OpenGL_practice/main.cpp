// TODO (Merlin): Statically link included libs to be packaged with final submission

#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "isosphere.h"
#include "model.h"

#define PI 3.141592654

/*const GLfloat cube[] =
    {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };
    const GLfloat colour_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
    };*/

GLuint vertexBuffer = 0;
GLuint colourBuffer = 0;
GLuint shaderProgramID;

void render(int n_vertices)
{
}

void setup_geometry(const GLfloat* vertex_buffer_data, const GLfloat* colour_buffer_data, const unsigned int n_vertices)
{
    if(!vertexBuffer)
        glGenBuffers(1, &vertexBuffer);
    if(!colourBuffer)
        glGenBuffers(1, &colourBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(GLfloat), colour_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void load_shaders(const char* vertex_src_path, const char* fragment_src_path)
{
    //Create shaders

    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    //Read Vertex Shader source code
    std::string vertexShaderCode = "";
    std::ifstream vertexShaderStream(vertex_src_path, std::ios::in);
    if(!vertexShaderStream.is_open())
        std::cerr << "Cannot load vertex shader code from" << vertex_src_path << std::endl;
    else
    {
        std::cout << "Loading vertex shader: " << vertex_src_path << std::endl;
        std::string line = "";
        while(getline(vertexShaderStream, line))
            vertexShaderCode += "\n" + line;
        vertexShaderStream.close();
    }

    //Read Fragment Shader source code
    std::string fragmentShaderCode = "";
    std::ifstream fragmentShaderStream(fragment_src_path, std::ios::in);
    if(!fragmentShaderStream.is_open())
        std::cerr << "Cannot load fragment shader code from" << fragment_src_path << std::endl;
    else
    {
        std::cout << "Loading fragment shader: " << fragment_src_path << std::endl;
        std::string line = "";
        while(getline(fragmentShaderStream, line))
            fragmentShaderCode += "\n" + line;
        fragmentShaderStream.close();
    }

    //For shader compilation checking
    GLint result = GL_FALSE;
    int infoLogLength;

    //Compile Vertex Shader
    std::cout << "Compiling Vertex Shader" << std::endl;
    char const* vertexSrcPtr = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSrcPtr, NULL);
    glCompileShader(vertexShaderID);

    //Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char vertexShaderErrMsg[infoLogLength];
    glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, vertexShaderErrMsg);
    if(vertexShaderErrMsg[0] != '\0')
        std::cerr << vertexShaderErrMsg << std::endl;

    //Compile Fragment Shader
    std::cout << "Compiling Fragment Shader" << std::endl;
    char const* fragmentSrcPtr = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSrcPtr, NULL);
    glCompileShader(fragmentShaderID);

    //Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char fragmentShaderErrMsg[infoLogLength];
    glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, fragmentShaderErrMsg);
    if(fragmentShaderErrMsg[0] != '\0')
        std::cerr << fragmentShaderErrMsg << std::endl;

    //Link program
    std::cout << "Linking program" << std::endl;
     shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);

    //Check program
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);
    glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
    char programErrMsg[infoLogLength];
    glGetProgramInfoLog(shaderProgramID, infoLogLength, NULL, programErrMsg);
    if(programErrMsg[0] != '\0')
        std::cerr << programErrMsg << std::endl;

    //Delete shaders
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void update_MVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 MVP = projection * view * model;

    const GLuint MVP_ID = glGetUniformLocation(shaderProgramID, "MVP");
    glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);
}
void check_input(GLFWwindow* window, glm::mat4& model, glm::mat4& view ,glm::mat4& projection, glm::vec3& camPos)
{
    glfwPollEvents();
    /*
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        model = glm::rotate(model, 0.5f, glm::vec3(0.0f ,1.0f ,0.0f));

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        model = glm::rotate(model, -0.5f, glm::vec3(0.0f ,1.0f ,0.0f));

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        model = glm::rotate(model, 0.5f, glm::vec3(1.0f ,0.0f ,0.0f));

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        model = glm::rotate(model, -0.5f, glm::vec3(1.0f ,0.0f ,0.0f));

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        model = glm::rotate(model, 0.5f, glm::vec3(0.0f ,0.0f ,1.0f));

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        model = glm::rotate(model, -0.5f, glm::vec3(0.0f ,0.0f ,1.0f));
*/
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        camPos.z += 0.05;// = glm::vec3(camPos.x, camPos.y, camPos.z + 0.05);
        view = glm::lookAt(camPos, glm::vec3(0,0,0), glm::vec3(0,1,0));
    }

    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        camPos.z -= 0.05;// glm::vec3(camPos.x, camPos.y, camPos.z - 0.05);
        view = glm::lookAt(camPos, glm::vec3(0,0,0), glm::vec3(0,1,0));
    }

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    update_MVP(model, view, projection);
}


int main(int argc, char **argv)
{
    Isosphere sphere1(0);
    Isosphere sphere2(5);

    GLfloat* sphere1Verts = sphere1.get_vertices();

    std::cout << "Number of faces:" << sphere2.get_n_vertices() /9 << std::endl;

    GLfloat colours1[sphere1.get_n_vertices()];
    for(int i=0; i<sphere1.get_n_vertices(); i++)
    {
        GLfloat col = ((float)(std::rand()%100)) / 100.0;
        colours1[i] = col;
    }

    GLfloat colours2[sphere2.get_n_vertices()];
    for(int i=0; i<sphere2.get_n_vertices(); i++)
    {
        GLfloat col = ((float)(std::rand()%100)) / 100.0;
        colours2[i] = col;
    }

    GLfloat UVs[sphere1.get_n_vertices()][2];
    for(int i=0; i<sphere1.get_n_vertices(); i+=3)
    {
        UVs[i][0] = 0.5 + atan2(sphere1Verts[i+2], sphere1Verts[i]) / (2*PI);
        UVs[i][1] = 0.5 + asin(sphere1Verts[i+1])/PI;
    }


    //Initialise GLFW
    if(!glfwInit())
    {
        std::cerr << "GLFW initialisation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x Anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(1024, 720, "Icosphere", NULL, NULL);

    //Check mainWindow
    if(mainWindow == NULL)
    {
        std::cout << "GLFW failed to create mainWindow" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(mainWindow);

    //Initialise GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW initialisation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    GLuint vertexArrayID;
    glGenVertexArrays(2, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    load_shaders("shader.vert", "shader.frag");
    glUseProgram(shaderProgramID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ///texture from http://podet.imcce.fr/podet_exp/app_dev.php/images/3229f30_Color%20Map.smaller_1.jpg
    const char* globe_tex_path = "textures/globe.tga";
    //GLuint loadTGA_glfw(globe_tex_path);

    glm::vec3 camPos(0,0,5);
    int angle = 0;

    glm::mat4 projection = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(camPos, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 model = glm::rotate((float)angle, glm::vec3(0,1,0)) * glm::translate(glm::vec3(2,0,0));
    glm::mat4 model1 = glm::rotate((float)angle, glm::vec3(0,1,0)) * glm::translate(glm::vec3(2,0,0));
    glm::mat4 model2 = glm::rotate((float)angle, glm::vec3(0,1,0)) * glm::translate(glm::vec3(-2,0,0));

    do
    {
        glfwPollEvents();

        angle = (angle + 1) % 360;
        model1 = glm::rotate((float)angle, glm::vec3(0,1,0)) * glm::translate(glm::vec3(2,0,0));
        model2 = glm::rotate((float)angle, glm::vec3(0,1,0)) * glm::translate(glm::vec3(-2,0,0));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update_MVP(model1, view, projection);
        setup_geometry(sphere1.get_vertices(), colours1, sphere1.get_n_vertices());
        glDrawArrays(GL_TRIANGLES, 0, sphere1.get_n_vertices());

        update_MVP(model2, view, projection);
        setup_geometry(sphere2.get_vertices(), colours2, sphere2.get_n_vertices());
        glDrawArrays(GL_TRIANGLES, 0, sphere2.get_n_vertices());

        glfwSwapBuffers(mainWindow);

        check_input(mainWindow, model1, view, projection, camPos);

        glFlush();
    }while(!glfwWindowShouldClose(mainWindow));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    return 0;
}
