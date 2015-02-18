#include "model.h"

Model::Model(glm::mat4 model_matrix)
{
    modelMatrix = model_matrix;
}

Model::Model(GLfloat* verts, GLfloat* cols, unsigned int n_vertices, glm::mat4 model_matrix)
{
    set_vertices(verts, n_vertices);
    set_vertices(cols, n_vertices);
    modelMatrix = model_matrix;
}

void Model::set_vertices(GLfloat* vert_arry, unsigned int new_nVertices)
{
    nVertices = new_nVertices;
    vertices.clear();
    for(int i=0; i<nVertices; i++)
        vertices.push_back(GLfloat(vert_arry[i]));
}

void Model::set_colours(GLfloat* col_arry, unsigned int new_nVertices)
{
    nVertices = new_nVertices;
    colours.clear();
    for(int i=0; i<nVertices; i++)
        colours.push_back(GLfloat(col_arry[i]));
}

void Model::set_model_matrix(glm::mat4 model_matrix)
{
    modelMatrix = model_matrix;
}

GLfloat* Model::get_vertices()
{
    return (GLfloat*) &vertices[0];
}

GLfloat* Model::get_colours()
{
    return (GLfloat*) &colours[0];
}

glm::mat4 Model::get_model_matrix()
{
    return modelMatrix;
}

unsigned int Model::get_n_vertices()
{
    return nVertices;
}
