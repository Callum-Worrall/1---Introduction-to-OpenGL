#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "glm_include.h"

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct VertexTexCoord
{
	vec4 position;
	vec2 tex_coord;
};

#endif