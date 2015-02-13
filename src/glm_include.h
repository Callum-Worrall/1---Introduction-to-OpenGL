#ifndef __GLM_INCLUDE_H__
#define __GLM_INCLUDE_H__

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct OpenGLData
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_index_count;
};

#endif