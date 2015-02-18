#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#define GLFW_DLL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint vbo[2]; /* Create handles for two Vertex Buffer Objects */
const GLfloat diamond[4][2] = {  /* We're going to create a simple diamond made from lines */
        {  0.0,  0.5  },  /* Top point    */
        {  0.5,  0.0  },  /* Right point  */
        {  0.0, -0.5  },  /* Bottom point */
        { -0.5,  0.0  }
    }; /* Left point   */
const GLfloat colors[4][3] = {
        {  1.0,  0.0,  0.0  },  /* Red   */
        {  0.0,  1.0,  0.0  },  /* Green */
        {  0.0,  0.0,  1.0  },  /* Blue  */
        {  0.0,  1.0,  1.0  }
    }; /* White */
GLchar *vertexsource, *fragmentsource; /* These pointers will receive the contents of our shader source code files */
GLuint vertexshader, fragmentshader; /* These are handles used to reference the shaders */
GLuint shaderprogram; /* This is a handle to the shader program */

char* filetobuf(char *file) { /* A simple function that will read a file into an allocated char pointer buffer */
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

void check(char *where) { // Function to check OpenGL error status
    char * what;
    int err = glGetError(); //0 means no error
    if(err == 0) {
//            fprintf(stderr, "No error %s\n", where);
            return;
            }
    if(err == GL_INVALID_ENUM)
        what = "GL_INVALID_ENUM";
    else if(err == GL_INVALID_VALUE)
        what = "GL_INVALID_VALUE";
    else if(err == GL_INVALID_OPERATION)
        what = "GL_INVALID_OPERATION";
    else if(err == GL_INVALID_FRAMEBUFFER_OPERATION)
        what = "GL_INVALID_FRAMEBUFFER_OPERATION";
    else if(err == GL_OUT_OF_MEMORY)
        what = "GL_OUT_OF_MEMORY";
    else
        what = "Unknown Error";
    fprintf(stderr, "Error (%d) %s  at %s\n", err, what, where);
    exit(1);
    }

void SetupShaders(void) {
    char text[1000];
    int length;
    fprintf(stderr, "Setting up shaders\n"); /* Allocate and assign two Vertex Buffer Objects to our handle */
    vertexsource = filetobuf("2/tut2.vert"); /* Read our shaders into the appropriate buffers */
//    fprintf(stderr, "\n%s\n\n", vertexsource);
    fragmentsource = filetobuf("2/tut2.frag");
//    fprintf(stderr, "\n%s\n\n", fragmentsource);
    printf("Shader Code loaded\n");
    vertexshader = glCreateShader(GL_VERTEX_SHADER); /* Assign our handles a "name" to new shader objects */
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0); /* Associate the source code buffers with each handle */
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    glCompileShader(fragmentshader);/* Compile our shader objects */
    glCompileShader(vertexshader);
    shaderprogram = glCreateProgram();/* Assign our program handle a "name" */
    glAttachShader(shaderprogram, vertexshader); /* Attach our shaders to our program */
    glAttachShader(shaderprogram, fragmentshader);
    glLinkProgram(shaderprogram); /* Link our program */
    glGetProgramInfoLog(shaderprogram, 1000, &length, text); // Check for errors
    if(length > 0) {
            fprintf(stderr, "Validate Shader Program\n%s\n", text );
            fprintf(stderr, "\n%s\n", text);
            }
    glUseProgram(shaderprogram); /* Set it as being actively used */
    printf("All Compiled\n");
    }

void SetupGeometry(void) {
    /*
    Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
    Bind our second VBO as being the active buffer and storing vertex attributes (colors)
    Copy the color data from colors to our buffer
    Copy the vertex data from diamond to our buffer
    8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values
    12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values
    Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex
    Enable attribute index 0 as being used
    Enable attribute index 1 as being used
    Specify that our color data is going into attribute index 1, and contains three floats per vertex
    Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
    */
    fprintf(stderr, "Set up vertices\n");
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindAttribLocation(shaderprogram, 0, "in_Position");
    glBindAttribLocation(shaderprogram, 1, "in_Color");
    printf("All Finished\n");
    }  /* finished */

void Render(void) { /* Make our background black */
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    check("Test point");
    glFlush();
    }

int main( void ) {
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    if( !glfwInit() ) {
            printf("Failed to start GLFW\n");
            exit( EXIT_FAILURE );
            }
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
            glfwTerminate();
            printf("GLFW Failed to start\n");
            return -1;
            }
    /* Make the window's context current */
    glfwMakeContextCurrent(window); // IMPORTANT: Must be done so glew recognises OpenGL
    glewExperimental = GL_TRUE;
    int err = glewInit();
    if (GLEW_OK != err) {
            /* Problem: glewInit failed, something is seriously wrong. */
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            }
    fprintf(stderr, "Glew done\n");
    fprintf(stderr, "GL INFO %s\n", glGetString(GL_VERSION));
    SetupShaders();
    SetupGeometry();
    glClearColor(1., 1., 1., 1.);
    while( !glfwWindowShouldClose(window) ) {// Main loop
            Render();// OpenGL rendering goes here...
            glfwSwapBuffers(window);// Swap front and back rendering buffers
            glfwPollEvents();
            }
    glfwDestroyWindow(window);
    glfwTerminate();// Close window and terminate GLFW
    exit( EXIT_SUCCESS );// Exit program
    }
