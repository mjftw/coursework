#include "VertexBufferObject.h"


static
inline
bool
FpEq( float f1, float f2 )
{
	return f1 == f2;
}


GLuint
VertexBufferObject::AddVertex( GLfloat x, GLfloat y, GLfloat z )
{
	Key key( x, y, z );

	if( collapseCommonVertices )
	{
		PMap::iterator iter = PointMap.find( key );

		if( iter != PointMap.end() )
		{
			// if did find an entry, then this point is a duplicate of a previous one,
			// so, just use it:

			return iter->second;		// same as "return PointMap[ key ]"
		}
	}

	// if don't find an entry for this point, create one:

	if( verbose )
		fprintf( stderr, "This point is new\n" );

	struct Point pt = { x, y, z,  c_nx, c_ny, c_nz,  c_r, c_g, c_b,  c_s, c_t };
	PointVec.push_back( pt );
	int ptindex = (int)PointVec.size( ) - 1;
	if( collapseCommonVertices )
		PointMap[ key ] = ptindex;	// make a new entry
	return ptindex;
}


void
VertexBufferObject::CollapseCommonVertices( bool tf )
{
	collapseCommonVertices = tf;
}


void
VertexBufferObject::Draw( )
{
	int numPoints = (int) PointVec.size( );
	int numElements = (int) ElementVec.size( );

	if( ! hasVertices  ||  numPoints == 0  ||  numElements == 0 )
	{
		if( verbose )
			fprintf( stderr, "Don't have anything to Draw!\n" );
		return;
	}


	if( useBufferObjects )
	{
		if( isFirstDraw )
		{
			glGenBuffers( 1, &pbuffer );
			glBindBuffer( GL_ARRAY_BUFFER, pbuffer );
			glBufferData( GL_ARRAY_BUFFER, numPoints * sizeof(struct Point), NULL, GL_STATIC_DRAW );
			parray = (struct Point *) glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
			(void) memmove( &parray[0].x, &PointVec[0].x, numPoints * sizeof(struct Point) );

			// note: the memmove is the same as saying:
			// for( int i = 0; i < numPoints; i++ )
			// {
			//	parray[i].x = PointVec[i].x;
			//	parray[i].y = PointVec[i].y;
			//	parray[i].z = PointVec[i].z;
			//	parray[i].nx = PointVec[i].nx;
			//	parray[i].ny = PointVec[i].ny;
			//	parray[i].nz = PointVec[i].nz;
			//	parray[i].r = PointVec[i].r;
			//	parray[i].g = PointVec[i].g;
			//	parray[i].b = PointVec[i].b;
			//	parray[i].s = PointVec[i].s;
			//	parray[i].t = PointVec[i].t;
			// }


			glUnmapBuffer( GL_ARRAY_BUFFER );
			parray = NULL;

			glGenBuffers( 1, &ebuffer );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, numElements * sizeof(GLuint), NULL, GL_STATIC_DRAW );
			earray = (GLuint *) glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY );
			for( int i = 0; i < numElements; i++ )
			{
				earray[i] = ElementVec[i];
			}
			glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER );
			earray = NULL;

			isFirstDraw = false;
		}

		glBindBuffer( GL_ARRAY_BUFFER, pbuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

		glVertexPointer( THREE_VALUES, GL_FLOAT, sizeof(struct Point), ELEMENT_OFFSET( &parray[0].x, &parray[0].x ) );
		glNormalPointer( GL_FLOAT, sizeof(struct Point), ELEMENT_OFFSET( &parray[0].x, &parray[0].nx ) );
		glColorPointer( THREE_VALUES, GL_FLOAT, sizeof(struct Point), ELEMENT_OFFSET( &parray[0].x, &parray[0].r ) );
		glTexCoordPointer( TWO_VALUES, GL_FLOAT, sizeof(struct Point), ELEMENT_OFFSET( &parray[0].x, &parray[0].s ) );

		glEnableClientState( GL_VERTEX_ARRAY );
		if( hasNormals )	glEnableClientState( GL_NORMAL_ARRAY );
		if( hasColors )		glEnableClientState( GL_COLOR_ARRAY );
		if( hasTexCoords )	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glDrawElements( topology, numElements, GL_UNSIGNED_INT, BUFFER_OFFSET( 0 ) );

		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	else
	{
		if( isFirstDraw )
		{
			// create the vertex array:
			parray = new struct Point [ numPoints ];
			(void) memmove( &parray[0].x, &PointVec[0].x, numPoints * sizeof(struct Point) );

			// see above comment about the memmove...

			// create the element array:
			earray = new GLuint [ numElements ];
			for( int i = 0; i < numElements; i++ )
			{
				earray[i] = ElementVec[i];
			}
			isFirstDraw = false;
		}

		glVertexPointer( THREE_VALUES, GL_FLOAT, sizeof(struct Point), &parray[0].x );
		glNormalPointer( GL_FLOAT, sizeof(struct Point), &parray[0].nx );
		glColorPointer( THREE_VALUES, GL_FLOAT, sizeof(struct Point), &parray[0].r );
		glTexCoordPointer( TWO_VALUES, GL_FLOAT, sizeof(struct Point), &parray[0].s );

		glEnableClientState( GL_VERTEX_ARRAY );
		if( hasNormals )	glEnableClientState( GL_NORMAL_ARRAY );
		if( hasColors )		glEnableClientState( GL_COLOR_ARRAY );
		if( hasTexCoords )	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glDrawElements( topology, numElements, GL_UNSIGNED_INT, earray );
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}


void
VertexBufferObject::glBegin( GLenum _topology )
{
	topology = _topology;
	Reset( );
	glBeginWasCalled = true;
}

void
VertexBufferObject::glEnd( )
{
	// doesn't need to do anything, but looks wrong if this isn't here
}


void
VertexBufferObject::glVertex3f( GLfloat x, GLfloat y, GLfloat z )
{
	hasVertices = true;
	int element = AddVertex( x, y, z );
	ElementVec.push_back( element );
}


void
VertexBufferObject::glVertex3fv( GLfloat *xyz )
{
	glVertex3f( xyz[0], xyz[1], xyz[2] );
}


void
VertexBufferObject::glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz )
{
	hasNormals = true;
	c_nx = nx;	c_ny = ny;	c_nz = nz;
}


void
VertexBufferObject::glNormal3fv( GLfloat *nxyz )
{
	glNormal3f( nxyz[0], nxyz[1], nxyz[2] );
}


void
VertexBufferObject::glColor3f( GLfloat r, GLfloat g, GLfloat b )
{
	hasColors = true;
	c_r = r;	c_g = g;	c_b = b;
}


void
VertexBufferObject::glColor3fv( GLfloat *rgb )
{
	glColor3f( rgb[0], rgb[1], rgb[2] );
}


void
VertexBufferObject::glTexCoord2f( GLfloat s, GLfloat t )
{
	hasTexCoords = true;
	c_s = s;	c_t = t;
}


void
VertexBufferObject::glTexCoord2fv( GLfloat *st )
{
	glTexCoord2f( st[0], st[1] );
}


void
VertexBufferObject::Print( FILE *fpout )
{
	char *blanks = "       ";

	fprintf( fpout, "\nDrawing %d points\n", PointVec.size( ) );
	fprintf( fpout, "    X       Y       Z  " );
	if( hasNormals )
		fprintf( fpout, "   Nx      Ny      Nz  " );
	else
		fprintf( fpout, "%7s %7s %7s", blanks, blanks, blanks );
	if( hasColors )
		fprintf( fpout, "    R       G       B  " );
	else
		fprintf( fpout, "%7s %7s %7s", blanks, blanks, blanks );
	if( hasTexCoords )
		fprintf( fpout, "    S       T  " );
	else
		fprintf( fpout, "%7s %7s", blanks, blanks );
		fprintf( fpout, "\n" );

	for( int i = 0; i < (int)PointVec.size(); i++ )
	{
		fprintf( fpout, "%7.2f %7.2f %7.2f", PointVec[i].x, PointVec[i].y, PointVec[i].z );
		if( hasNormals )
			fprintf( fpout, "%7.2f %7.2f %7.2f", PointVec[i].nx, PointVec[i].ny, PointVec[i].nz );
		else
			fprintf( fpout, "%7s %7s %7s", blanks, blanks, blanks );
		if( hasColors )
			fprintf( fpout, "%7.2f %7.2f %7.2f", PointVec[i].r, PointVec[i].g, PointVec[i].b );
		else
			fprintf( fpout, "%7s %7s %7s", blanks, blanks, blanks );
		if( hasTexCoords )
			fprintf( fpout, "%7.2f %7.2f", PointVec[i].s, PointVec[i].t );
		else
			fprintf( fpout, "%7s %7s", blanks, blanks );
		fprintf( fpout, "\n" );
	}

	int numPerLine = 6;
	if( topology == GL_TRIANGLES )
		numPerLine = 3;
	else if( topology == GL_QUADS )
		numPerLine = 4;

	fprintf( fpout, "\n" );
	fprintf( fpout, "Drawing %d array elements:\n", ElementVec.size( ) );
	int count = 0;
	for( int i = 0; i < (int)ElementVec.size( ); i++ )
	{
		if( ElementVec[i] == RESTART_INDEX )
			fprintf( fpout, "RESTART" );
		else
			fprintf( fpout, " %6d", ElementVec[i] );
		count++;
		if( (count % numPerLine) == 0 )
			fprintf( fpout, "\n" );
	}
}


void
VertexBufferObject::Reset( )
{
	isFirstDraw = true;
	hasVertices = hasNormals = hasColors = hasTexCoords = false;
	glPrimitiveRestartIndex( VertexBufferObject::RESTART_INDEX );
	glEnable( GL_PRIMITIVE_RESTART );

	if( parray != NULL )
	{
		delete [ ] parray;
		parray = NULL;
	}
	if( earray != NULL )
	{
		delete [ ] earray;
		earray = NULL;
	}
	if( pbuffer != 0 )
	{
		glDeleteBuffers( 1, &pbuffer );
		pbuffer = 0;
	}
	if( ebuffer != 0 )
	{
		glDeleteBuffers( 1, &ebuffer );
		ebuffer = 0;
	}

	PointVec.clear( );
	PointMap.clear( );
	ElementVec.clear( );
}


void
VertexBufferObject::RestartPrimitive( )
{
	ElementVec.push_back( RESTART_INDEX );
}


void
VertexBufferObject::SetTol( float _tol )
{
	tol = fabs( _tol );
}


void
VertexBufferObject::SetVerbose( bool v )
{
	verbose = v;
}


void
VertexBufferObject::UseBufferObjects( bool tf )
{
	useBufferObjects = tf;

	// be sure that vbo's are supported here before we try using them:

	if( useBufferObjects   &&   ! IsExtensionSupported( "GL_ARB_vertex_buffer_object" ) )
	{
		fprintf( stderr, "Vertex Buffer Objects are not supported on this system!\n" );
		useBufferObjects = false;
	}
}


// these are here to make the map functions work:
// (Do an L1 test for tolerance equality -- presume it's faster than an L2 sqrt)

bool
operator< ( const Key& k0, const Key& k1 )
{
	if( k0.x < k1.x )
		return true;

	if( k0.x > k1.x )
		return false;

	if( k0.y < k1.y )
		return true;

	if( k0.y > k1.y )
		return false;

	return  k0.z < k1.z;
};

bool operator== ( const Key& k0, const Key& k1 )
{
	return  FpEq(k0.x,k1.x)  &&  FpEq(k0.y,k1.y)  &&  FpEq(k0.z,k1.z);
};



bool
IsExtensionSupported( const char *extension )
{
	// see if the extension is bogus:

	if( extension == NULL  ||  extension[0] == '\0' )
		return false;

	GLubyte *where = (GLubyte *) strchr( extension, ' ' );
	if( where != 0 )
		return false;

	// get the full list of extensions:

	const GLubyte *extensions = glGetString( GL_EXTENSIONS );

	for( const GLubyte *start = extensions; ; )
	{
		where = (GLubyte *) strstr( (const char *) start, extension );
		if( where == 0 )
			return false;

		GLubyte *terminator = where + strlen(extension);

		if( where == start  ||  *(where - 1) == ' ' )
			if( *terminator == ' '  ||  *terminator == '\0' )
				return true;
		start = terminator;
	}
	return false;
}
