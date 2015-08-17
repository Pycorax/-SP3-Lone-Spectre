#ifndef LOAD_TGA_H
#define LOAD_TGA_H

#include <GL\glew.h>

GLuint LoadTGA(const char *file_path);
void ClearTGA(unsigned textureID);

#endif