#ifndef __UTILITY_H__
#define __UTILITY_H__

#pragma once
#include "glm_include.h"
#include "gl_core_4_4.h"


class Utility
{
public:

	static bool LoadShader(char* vertex_filename, char* fragment_filename, GLuint* result);

};

#endif