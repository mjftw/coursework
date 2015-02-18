#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <ctype.h>
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

//#ifdef WIN32
//#include <windows.h>
//#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include "glut.h"
#include <map>
#include <stdarg.h>

#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER	0x91B9
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferObject.h"

inline int GetOSU( int flag )
{
	int i;
	glGetIntegerv( flag, &i );
	return i;
}


void	CheckGlErrors( const char* );



class GLSLProgram
{
  private:
	std::map<const char *, int>	AttributeLocs;
	const char *			Cfile;
	unsigned int		Cshader;
	const char *			Ffile;
	unsigned int		Fshader;
	const char *			Gfile;
	GLuint			Gshader;
	bool			IncludeGstap;
	GLenum			InputTopology;
	GLenum			OutputTopology;
	GLuint			Program;
	const char *			TCfile;
	GLuint			TCshader;
	const char *			TEfile;
	GLuint			TEshader;
	std::map<const char *, int>	UniformLocs;
	bool			Valid;
	const char *			Vfile;
	GLuint			Vshader;
	bool			Verbose;

	static int		CurrentProgram;

	void	AttachShader( GLuint );
	bool	CanDoBinaryFiles;
	bool	CanDoComputeShaders;
	bool	CanDoFragmentShaders;
	bool	CanDoGeometryShaders;
	bool	CanDoTessControlShaders;
	bool	CanDoTessEvaluationShaders;
	bool	CanDoVertexShaders;
	int	    CompileShader( GLuint );
	bool	CreateHelper( const char *, ... );
	int	    GetAttributeLocation( const char * );
	int	    GetUniformLocation( const char * );


  public:
    GLSLProgram( );

	bool	Create( const char *, const char * = NULL, const char * = NULL, const char * = NULL, const char * = NULL, const char * = NULL );
	void	DispatchCompute( GLuint, GLuint = 1, GLuint = 1 );
	bool    Create_from_GLSL(const char *);
	bool	IsExtensionSupported( const char * );
	bool	IsNotValid( );
	bool	IsValid( );
	void	LoadBinaryFile( const char * );
	void	LoadProgramBinary( const char *, GLenum );
	void	SaveBinaryFile( const char * );
	void	SaveProgramBinary( const char *, GLenum * );
#ifdef VERTEX_BUFFER_OBJECT_H
	void	SetAttribute( const char *, VertexBufferObject&, GLuint );
#endif
	void	SetGstap( bool );
	void	SetInputTopology( GLenum );
	void	SetOutputTopology( GLenum );
	void	SetUniform( const char *, int );
	void	SetUniform( const char *, unsigned int );
	void	SetUniform( const char *, float );
	void	SetUniform( const char *, float, float, float );
	void	SetUniform( const char *, float[3] );
	void	SetUniform( const char *, glm::vec3 );
	void	SetUniform( const char *, glm::vec4 );
    void    SetUniform( const char *, glm::mat3);
    void    SetUniform( const char *, glm::mat4);
	void	SetVerbose( bool );
	void	Use( );
	void	Use( GLuint );
	void	UseFixedFunction( );
};

#endif		// #ifndef GLSLPROGRAM_H
