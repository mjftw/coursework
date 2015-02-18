#include "isosphere.h"

Isosphere::Isosphere()
{
    generate_icosahedron();
}

Isosphere::Isosphere(unsigned int n_iterations)
{
    generate_icosahedron();

    if(n_iterations > 5)
        n_iterations = 5;

    for(int i=0; i<n_iterations; i++)
        subdivide();
}

Isosphere::~Isosphere()
{
}

void Isosphere::generate_icosahedron()
{
    ///Pdf used for Icosahedron vertices can be found at:
    ///http://www.classes.cs.uchicago.edu/archive/2003/fall/23700/docs/handout-04.pdf

    const float t = (1 + sqrt(5.0)) / 2; // Tao/Phi, golden ratio
    const float u = 1.0 / sqrt(1 + t*t); //Scale each point to be on unit sphere

    const GLfloat icosahedron_vertices[12][3] =
    {
        { t*u,  1*u,  0},    //V0
        {-t*u,  1*u,  0},    //V1
        { t*u, -1*u,  0},    //V2
        {-t*u, -1*u,  0},    //V3
        { 1*u,  0,    t*u},  //V4
        { 1*u,  0,   -t*u},  //V5
        {-1*u,  0,    t*u},  //V6
        {-1*u,  0,   -t*u},  //V7
        { 0,    t*u,  1*u},  //V8
        { 0,   -t*u,  1*u},  //V9
        { 0,    t*u, -1*u},  //V10
        { 0,   -t*u, -1*u}   //V11
    };
    const int icosahedron_triangles[20][3] =
    {
        {0  ,8  ,4},      //T0
        {0  ,5  ,10},     //T1
        {2  ,4  ,9},      //T2
        {2  ,11 ,5},      //T3
        {1  ,6  ,8},      //T4
        {1  ,10 ,7},      //T5
        {3  ,9  ,6},      //T6
        {3  ,7  ,11},     //T7
        {0  ,10 ,8},      //T8
        {1  ,8  ,10},     //T9
        {2  ,9  ,11},     //T10
        {3  ,9  ,11},     //T11
        {4  ,2  ,0},      //T12
        {5  ,0  ,2},      //T13
        {6  ,1  ,3},      //T14
        {7  ,3  ,1},      //T15
        {8  ,6  ,4},      //T16
        {9  ,4  ,6},      //T17
        {10 ,5  ,7},      //T18
        {11 ,7  ,5}       //T19
    };

    Triangle temp;

    for(int i=0; i<20; i++)         //Faces
    {
        for(int j=0; j<3; j++)      //Vertices
            for(int k=0; k<3; k++)  //XYZ]
            {
                temp[j][k] = icosahedron_vertices[icosahedron_triangles[i][j]][k];
            }
       faces.push_back(Triangle(temp));
    }
}

GLfloat* Isosphere::get_vertices()
{
    vertices.clear();
    for(int i=0; i<faces.size(); i++)
        for(int j=0; j<3; j++)
            for(int k=0; k<3; k++)
                vertices.push_back(faces[i][j][k]);

    return &vertices[0];
}

const unsigned int Isosphere::get_n_vertices()
{
    return faces.size()*3*3;
}
Isosphere::Triangle::Triangle()
{
}
Isosphere::Triangle::Triangle(GLfloat v0px, GLfloat v0py, GLfloat v0pz,
                              GLfloat v1px, GLfloat v1py, GLfloat v1pz,
                              GLfloat v2px, GLfloat v2py, GLfloat v2pz)
{
    V0.x = -v0px;
    V0.y = -v0py;
    V0.z = -v0pz;
    V1.x = -v1px;
    V1.y = -v1py;
    V1.z = -v1pz;
    V2.x = -v2px;
    V2.y = -v2py;
    V2.z = -v2pz;
}

void Isosphere::subdivide()
{
    subfaces.clear();
    for(int i=0; i<faces.size(); i++)
    {
        Triangle sub0, sub1, sub2, sub3;
        faces[i].subdivide(sub0, sub1, sub2, sub3);
        subfaces.push_back(sub0);
        subfaces.push_back(sub1);
        subfaces.push_back(sub2);
        subfaces.push_back(sub3);
    }
    faces.clear();
    faces = subfaces;
}

GLfloat* Isosphere::get_normals() ///NOT WORKING AT CURRENT
{
    Triangle::Point normal_vec, face_normal;
    normals.clear();
    int no_faces_around_vertex = 0;
    for(int i=0; i<faces.size(); i++)//for each triangle
        for(int j=0; j<3; j++)//for each vertex
        {
            faces[i].get_normal(normal_vec);

            for(int k=0; k<faces.size(); k++)//for each triangle
                for(int l=0; l<3; l++)//for each vertex
                {
                    if(faces[i][j] == faces[k][l])
                    {
                        faces[k].get_normal(face_normal);
                        normal_vec.x += face_normal.x;
                        normal_vec.y += face_normal.y;
                        normal_vec.z += face_normal.z;
                        no_faces_around_vertex++;
                    }
                }
            normal_vec.x /= no_faces_around_vertex;
            normal_vec.y /= no_faces_around_vertex;
            normal_vec.z /= no_faces_around_vertex;
            no_faces_around_vertex = 0;

            normals.push_back(normal_vec);
        }
    return (GLfloat*) &normals[0];
}

void Isosphere::Triangle::subdivide(Triangle& sub0, Triangle& sub1, Triangle& sub2, Triangle& sub3)
{
    /*Subfaces:   Vertices & Midpoints:
       /0\                V0
      /___\               / \
     /1\3 /2\          M1/___\M0
    /___\/___\          / \  / \
                     V1/___\/___\V2
                           M2
    */
    Point M0, M1, M2;
    GLfloat dist_to_origin;

    M0.x = (V2.x + V0.x)/2;
    M0.y = (V2.y + V0.y)/2;
    M0.z = (V2.z + V0.z)/2;

    //Scale points to be on unit sphere
    dist_to_origin = sqrt(M0.x*M0.x + M0.y*M0.y + M0.z*M0.z);
    M0.x /= dist_to_origin;
    M0.y /= dist_to_origin;
    M0.z /= dist_to_origin;

    M1.x = (V0.x + V1.x)/2;
    M1.y = (V0.y + V1.y)/2;
    M1.z = (V0.z + V1.z)/2;

    //Scale points to be on unit sphere
    dist_to_origin = sqrt(M1.x*M1.x + M1.y*M1.y + M1.z*M1.z);
    M1.x /= dist_to_origin;
    M1.y /= dist_to_origin;
    M1.z /= dist_to_origin;

    M2.x = (V1.x + V2.x)/2;
    M2.y = (V1.y + V2.y)/2;
    M2.z = (V1.z + V2.z)/2;

    //Scale points to be on unit sphere
    dist_to_origin = sqrt(M2.x*M2.x + M2.y*M2.y + M2.z*M2.z);
    M2.x /= dist_to_origin;
    M2.y /= dist_to_origin;
    M2.z /= dist_to_origin;


    //Subface 0
    sub0.V0 = V0;
    sub0.V1 = M1;
    sub0.V2 = M0;

    //Subface 1
    sub1.V0 = M1;
    sub1.V1 = V1;
    sub1.V2 = M2;

    //Subface 2
    sub2.V0 = M0;
    sub2.V1 = M2;
    sub2.V2 = V2;

    //Subface 3
    sub3.V0 = M0;
    sub3.V1 = M1;
    sub3.V2 = M2;
}

void Isosphere::Triangle::get_normal(Point& norm_vector)
{
    //vectors from sides V0 to V1 (a), and V0 to V2 (b)
    Point a, b; //a, b
    a.x = V0.x-V1.x;
    a.y = V0.y-V1.y;
    a.z = V0.z-V1.z;

    b.x = V0.x-V2.x;
    b.y = V0.y-V2.y;
    b.z = V0.z-V2.z;

    //Cross product
    norm_vector.x = a.y*b.z - a.z*b.y;
    norm_vector.y = a.z*b.x - a.x*b.z;
    norm_vector.z = a.x*b.y - a.y*b.x;

    //Normalise
    int magnitude = sqrt(norm_vector.x*norm_vector.x + norm_vector.y*norm_vector.y + norm_vector.z*norm_vector.z);
    norm_vector.x /= magnitude;
    norm_vector.y /= magnitude;
    norm_vector.z /= magnitude;
}








