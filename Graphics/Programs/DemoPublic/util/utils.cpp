#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

#include "utils.h"

void Check(const char *where) { // Function to check OpenGL error status
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

    // useful stuff



//#endif



/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "r"); /* Open file for reading */
    if (!fptr){
        fprintf(stderr, "%s not found\n", file);
        exit(0);
    } /* Return NULL on failure */
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file plus a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

