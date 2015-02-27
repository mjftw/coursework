#ifndef ISOSPHERE_H
#define ISOSPHERE_H

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

class Isosphere
{
    private:
        class Triangle
        {
            public:
                Triangle();
                Triangle(GLfloat v0px, GLfloat v0py, GLfloat v0pz,
                         GLfloat v1px, GLfloat v1py, GLfloat v1pz,
                         GLfloat v2px, GLfloat v2py, GLfloat v2pz);

                class Point
                {
                    public:
                        Point()
                        {
                            x = 0.0;
                            y = 0.0;
                            z = 0.0;
                        }

                        GLfloat x;
                        GLfloat y;
                        GLfloat z;

                        GLfloat& operator[](int index)
                        {
                            if(index == 0)
                                return x;
                            else if(index == 1)
                                return y;
                            else if(index == 2)
                                return z;
                            else
                                std::cerr << "ERROR: Triangle index out of bounds" << std::endl;

                            return out_of_bounds;
                        }

                        const bool operator==(Point& p2)
                        {
                            if((x == p2.x) && (y == p2.y) && (z == p2.z))
                                return true;
                            else
                                return false;
                        }

                    private:
                        GLfloat out_of_bounds;
                };

                Point V0;
                Point V1;
                Point V2;

                Triangle subdivide();

                Point& operator[](int index)
                {
                    if(index == 0)
                        return V0;
                    else if(index == 1)
                        return V1;
                    else if(index == 2)
                        return V2;
                    else
                        std::cerr << "ERROR: Triangle index out of bounds" << std::endl;

                    return out_of_bounds;
                }

                void subdivide(Triangle&, Triangle&, Triangle&, Triangle&);
                void get_normal(Point&);
                void get_normal_line(Point& pt1, Point& pt2, GLfloat length);
            private:
                Point out_of_bounds;
        };
    public:
        Isosphere();
        Isosphere(unsigned int n_iterations);
        virtual ~Isosphere();
        GLfloat* get_vertices();
        const unsigned int get_n_vertices();
        GLfloat* get_normals();
        GLfloat* get_normal_lines(GLfloat length = 0.25);
        const unsigned int get_n_normal_lines();
        void subdivide();
    private:

        std::vector<Triangle> faces;
        std::vector<Triangle> subfaces;
        std::vector<Triangle::Point> normals;
        std::vector<GLfloat> normal_lines;
        std::vector<GLfloat> vertices;
        void generate_icosahedron();


};
#endif // ISOSPHERE_H

