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


void print(glm::vec2 x) {
    printf("[%f %8.4f]\n", x[0], x[1]);
}
void print(glm::vec3 x) {
    printf("[%8.4f %8.4f %8.4f]\n", x[0], x[1], x[2]);
}

void print(glm::vec4 x) {
    printf("[%8.4f %8.4f %8.4f %8.4f]\n", x[0], x[1], x[2], x[3]);
}

void print(glm::mat4 x) {
    printf("[[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]]\n",
           x[0][0], x[0][1], x[0][2], x[0][3],
           x[1][0], x[1][1], x[1][2], x[1][3],
           x[2][0], x[2][1], x[2][2], x[2][3],
           x[3][0], x[3][1], x[3][2], x[3][3]);
}

void print(glm::mat3 x) {
    printf("[[%8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f]]\n",
           x[0][0], x[0][1], x[0][2],
           x[1][0], x[1][1], x[1][2],
           x[2][0], x[2][1], x[2][2]);
}

void print(glm::mat2 x) {
    printf("[[%8.4f %8.4f]\n[%8.4f %8.4f]]\n",
           x[0][0], x[0][1],
           x[1][0], x[1][1]);
}

void print(const char *x) {
    printf("%s\n", x);
}

void print(float x) {
    printf("%8.4f\n", x);
}
void print(int x) {
    printf("%d\n", x);
}

/*
 *  Without line feed.
 */

void Print(glm::vec2 x) {
    printf("[%8.4f %8.4f] ", x[0], x[1]);
}
void Print(glm::vec3 x) {
    printf("[%8.4f %8.4f %8.4f] ", x[0], x[1], x[2]);
}

void Print(glm::vec4 x) {
    printf("[%8.4f %8.4f %8.4f %8.4f] ", x[0], x[1], x[2], x[3]);
}

void Print(glm::mat4 x) {
    x = glm::transpose(x); // cos I got  the storage wrong, and its quicker than retyping.
    printf("\n[[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]]\n",
           x[0][0], x[0][1], x[0][2], x[0][3],
           x[1][0], x[1][1], x[1][2], x[1][3],
           x[2][0], x[2][1], x[2][2], x[2][3],
           x[3][0], x[3][1], x[3][2], x[3][3]);
}

void Print(glm::mat3 x) {
    x = glm::transpose(x); // cos I got  the storage wrong
    printf("\n[[%8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f]]\n",
           x[0][0], x[0][1], x[0][2],
           x[1][0], x[1][1], x[1][2],
           x[2][0], x[2][1], x[2][2]);
}

void Print(glm::mat2 x) {
    x = glm::transpose(x); // cos I got  the storage wrong
    printf("\n[[%8.4f %8.4f]\n[%8.4f %8.4f]]\n",
           x[0][0], x[0][1],
           x[1][0], x[1][1]);
}

void Print(const char *x) {
    printf("%s       ", x);
}

void Print(float x) {
    printf("%8.4f ", x);
}
void Print(int x) {
    printf("%8d ", x);
}
void Print(void) {
    printf("\n");
}

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


