#ifndef MODEL_H
#define MODEL_H

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

class Model
{
    public:
        Model(glm::mat4 model_matrix = glm::mat4(1.0f));
        Model(GLfloat* verts, GLfloat* cols, unsigned int n_vertices, glm::mat4 model_matrix = glm::mat4(1.0f));

        void set_vertices(GLfloat* vert_arry, unsigned int new_nVertices);
        void set_colours(GLfloat* vert_arry, unsigned int new_nVertices);
        void set_model_matrix(glm::mat4 model_matrix);

        GLfloat* get_vertices();
        GLfloat* get_colours();
        glm::mat4 get_model_matrix();
        unsigned int get_n_vertices();

    private:
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> colours;
        unsigned int nVertices;
        glm::mat4 modelMatrix;

};

#endif // MODEL_H
