#ifndef __UTILITY_H__
#define __UTILITY_H__

#pragma once

typedef unsigned int GLuint;

class Utility
{
public:


};


bool LoadShaderType(unsigned int shader_type, char* shader_pathname, unsigned int* output);

bool LoadShaders(char* vertex_filename, char* geometry_filename, char* fragment_filename, GLuint* result);

#endif