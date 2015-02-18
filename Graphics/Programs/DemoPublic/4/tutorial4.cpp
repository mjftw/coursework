/*
   This is a variation of tutorial3 using a single VBO for specifying the vertex
   attribute data; it is done by setting the VertexAttribPointer parameters
   "stride" and "pointer" to suitable values.
   In particular for the pointer parameter, macro "offsetof" should be used so to
   avoid problem with alignment and padding for different architecture.

   Modified to use GLM

   By consultit@katamail.com

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h> /* must include for the offsetof macro */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "utils.h"

int reverse = 1;
struct Vertex {
    GLdouble position[3];
    GLfloat color[3];
    };
/* These pointers will receive the contents of our shader source code files */
GLchar *vertexsource, *fragmentsource, *geometrysource;
/* These are handles used to reference the shaders */
GLuint vertexshader, fragmentshader, geometryshader;
/* This is a handle to the shader program */
GLuint shaderprogram;
GLuint vbo[1]; /* Create handles for our Vertex Array Object and One Vertex Buffer Object */

void check(char *where) { // Function to check OpenGL error status
    const char * what;
    int err = glGetError(); //0 means no error
    if(!err)
        return;
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


void SetupGeometry() {
    /* An array of 12 Vertices to make 4 coloured triangles in the shape of a tetrahedron*/
    const struct Vertex tetrahedron[12] = {
            {{  1.0,  1.0,  1.0  }, {  1.0f,  0.0f,  0.0f  }},
            {{ -1.0, -1.0,  1.0  }, {  1.0f,  0.0f,  0.0f  }},
            {{ -1.0,  1.0, -1.0  }, {  1.0f,  0.0f,  0.0f  }},
            {{  1.0,  1.0,  1.0  }, {  0.0f,  1.0f,  0.0f  }},
            {{ -1.0, -1.0,  1.0  }, {  0.0f,  1.0f,  0.0f  }},
            {{  1.0, -1.0, -1.0  }, {  0.0f,  1.0f,  0.0f  }},
            {{  1.0,  1.0,  1.0  }, {  0.0f,  0.0f,  1.0f  }},
            {{ -1.0,  1.0, -1.0  }, {  0.0f,  0.0f,  1.0f  }},
            {{  1.0, -1.0, -1.0  }, {  0.0f,  0.0f,  1.0f  }},
            {{ -1.0, -1.0,  1.0  }, {  1.0f,  1.0f,  1.0f  }},
            {{ -1.0,  1.0, -1.0  }, {  1.0f,  1.0f,  1.0f  }},
            {{  1.0, -1.0, -1.0  }, {  1.0f,  1.0f,  1.0f  }}
        };
    /* Allocate and assign One Vertex Buffer Object to our handle */
    glGenBuffers(1, vbo);
    /* Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    /* Copy the vertex data from tetrahedron to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 Vertex values */
    glBufferData ( GL_ARRAY_BUFFER, 12 * sizeof ( struct Vertex ), tetrahedron, GL_STATIC_DRAW );
    /* Specify that our coordinate data is going into attribute index 0, and contains three doubles per vertex */
    /* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0 */
    glVertexAttribPointer ( ( GLuint ) 0, 3, GL_DOUBLE, GL_FALSE,  sizeof ( struct Vertex ), ( const GLvoid* ) offsetof (struct Vertex, position) );
    /* Enable attribute index 0 as being used */
    glEnableVertexAttribArray(0);
    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    /* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
     occupied by the first attribute (position) */
    glVertexAttribPointer ( ( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, sizeof ( struct Vertex ), ( const GLvoid* ) offsetof(struct Vertex, color) ); // bug );
    /* Enable attribute index 1 as being used */
    glEnableVertexAttribArray ( 1 );/* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    }

void SetupShaders(void) {
    char text[1000];
    int length;

    /* Read our shaders into the appropriate buffers */
    vertexsource = filetobuf("4/tutorial4.vert");
    printf("vertex source\n %s\n", vertexsource);
    fragmentsource = filetobuf("4/tutorial4.frag");
    printf("fragment source\n %s\n", fragmentsource);
    geometrysource = filetobuf("4/tutorial4.geom");
    printf("geometry source\n %s\n", geometrysource);
    /* Assign our handles a "name" to new shader objects */
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    geometryshader = glCreateShader(GL_GEOMETRY_SHADER);

    /* Associate the source code buffers with each handle */
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    glShaderSource(geometryshader, 1, (const GLchar**)&geometrysource, 0);
    /* Compile our shader objects */
    glCompileShader(vertexshader);
    glCompileShader(fragmentshader);
    glCompileShader(geometryshader);
    /* Assign our program handle a "name" */

    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexshader);/* Attach our shaders to our program */
    glAttachShader(shaderprogram, fragmentshader);
    glAttachShader(shaderprogram, geometryshader);
    glBindAttribLocation(shaderprogram, 0, "in_Position"); /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
    glBindAttribLocation(shaderprogram, 1, "in_Color");
    glLinkProgram(shaderprogram);/* Link our program, and set it as being actively used */
    glGetProgramInfoLog(shaderprogram, 1000, &length, text);
    if(length > 0)
        fprintf(stderr, "Validate Shader Program\n%s\n", text );

    glUseProgram(shaderprogram);
    }

void Render(int i) {
    GLfloat angle;
    glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    angle = (GLfloat) (i % 360);
    glm::mat4 View = glm::mat4(1.);
    View = glm::translate(View, glm::vec3(0.f, 0.f, -15.0f));
    View = glm::rotate(View, angle * -1.0f, glm::vec3(1.f, 0.f, 0.f));
    View = glm::rotate(View, angle * 0.5f, glm::vec3(0.f, 1.f, 0.f));
    View = glm::rotate(View, angle * 0.5f, glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 Model = glm::mat4(1.0);
    glm::mat4 MVP = Projection * View * Model;
    glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
    check("ok");

    /* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    /* Invoke glDrawArrays telling that our data consists of individual triangles */
    }
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
        reverse = 1 - reverse; // togrls reverse from 0 to 1 to o to ...
    }

int main( void ) {
    int running = GL_TRUE;
    int k = 0;
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
    glfwSetKeyCallback(window, key_callback);
    fprintf(stderr, "GL INFO %s\n", glGetString(GL_VERSION));
    SetupShaders();
    SetupGeometry();
    glEnable(GL_DEPTH_TEST);
    printf("Starting\n");
    glClearColor(0.0, 0.0, 0.0, 1.0);/* Make our background black */
    while(!glfwWindowShouldClose(window)) {// Main loop
            Render(k);// OpenGL rendering goes here...
            if(reverse)
                k--;
            else
                k++;
            glfwSwapBuffers(window);// Swap front and back rendering buffers
            glfwPollEvents(); // Poll for events.

            }
    glfwTerminate();// Close window and terminate GLFW
    exit( EXIT_SUCCESS );// Exit program
    }
