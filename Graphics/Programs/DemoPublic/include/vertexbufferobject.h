#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

//#ifdef WIN32
//#include "windows.h"
//#endif
#include <GL/glew.h>

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <map>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>

#include "utils.h"

#define VERTEX_ATTRIBUTE 0
#define COLOR_ATTRIBUTE 1
#define NORMAL_ATTRIBUTE 2
#define TEXTURE_ATTRIBUTE 3

//#define BUFFER_OFFSET(bytes)	( (GLubyte *)NULL + (bytes) )
//#define ELEMENT_OFFSET(a1,a2)	(  BUFFER_OFFSET( (char *)(a2) - (char *)(a1) )  )

bool	IsExtensionSupported( const char * );



struct Point {
    float x, y, z;
    float nx, ny, nz;
    float r, g, b;
    float s, t;
};


class Key {
public:
    float x, y, z;

    Key( float _x, float _y, float _z ) {
        x = _x;
        y = _y;
        z = _z;
    };

    Key( float _v[3] ) {
        x = _v[0];
        y = _v[1];
        z = _v[2];
    };

    Key( const Key& k ) {
        x = k.x;
        y = k.y;
        z = k.z;
    };
};

typedef std::map< Key, int >	PMap;



class VertexBufferObject {
private:
    bool				hasVertices, hasNormals, hasColors, hasTexCoords;
    volatile float				c_r, c_g, c_b;
    volatile float				c_nx, c_ny, c_nz;
    volatile float				c_s, c_t;
    volatile float				c_x, c_y, c_z;

    GLenum				topology;
    bool				verbose;
    bool				useBufferObjects;
    bool				collapseCommonVertices;
    bool				isFirstDraw;
    bool				glBeginWasCalled;
    bool				drawWasCalled;


    std::vector <glm::vec2> textureContainer;
    std::vector <glm::vec3> colorContainer, normalContainer;
    std::vector <glm::vec4> vertexContainer;
    GLuint vertexBuffers[4];
    GLuint arrayBuffer;
    GLuint numVertices;


    float tol;	// how close two points need to be before we collapse them

    GLuint RESTART_INDEX;	// 0xffffffff
    int TWO_VALUES;
    int THREE_VALUES;

    void AddVertex( GLfloat, GLfloat, GLfloat );
    void Reset( );
    void Fillin();

public:
    std::map<int, int> vertexAtributesMap;
    const char *vboName;
    void CollapseCommonVertices( bool );
    void Draw( );
    void glBegin( GLenum );
    void glColor3f( GLfloat, GLfloat, GLfloat );
    void glColor3fv( GLfloat * );
    void glColor3fv( glm::vec3 );
    void glEnd( );
    void glNormal3f( GLfloat, GLfloat, GLfloat );
    void glNormal3fv( GLfloat * );
    void glNormal3fv( glm::vec3);
    void glTexCoord2f( GLfloat, GLfloat );
    void glTexCoord2fv( GLfloat * );
    void glTexCoord2fv( glm::vec2 );
    void glVertex3f( GLfloat, GLfloat, GLfloat );
    void glVertex3fv( GLfloat * );
    void glVertex3fv( glm::vec3 );
    void Print( FILE * = stderr );
    void makeObj( const char *name );
    void RestartPrimitive( );
    void SetTol( float );
    void SetVerbose( bool );
    void UseBufferObjects( bool );
    void SelectVAO();
    void DeSelectVAO();
    VertexBufferObject( ) {
        verbose = false;
        Reset( );
        tol = 0.;
        useBufferObjects = true;
        collapseCommonVertices = false;
        glBeginWasCalled = false;
        numVertices = -1;
        arrayBuffer = 0;
        for(int i=0; i< 4; i++)
            vertexBuffers[i] = 0;
        textureContainer.clear();
        vertexContainer.clear();
        colorContainer.clear();
        normalContainer.clear();
        vertexAtributesMap.clear();
        vertexAtributesMap[0] = 0;
        RESTART_INDEX = ~0;	// 0xffffffff
        TWO_VALUES   = 2;
        THREE_VALUES = 3;
        vboName = "Not named";

    };

    ~VertexBufferObject( ) {
        // be sure all dynamic arrays are deleted and all buffers are destroyed:
        // (it's possible to cause memory leaks in both cpu and gpu memory)

        Reset( );
    };
};
#endif
