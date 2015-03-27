#include "Utility.h"
#include <cstdio>

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "glm_include.h"
#include "Vertex.h"


bool LoadShaderType(
	char* shader_filename,
	GLenum shader_type,
	unsigned int* output)
{
	bool succeeded = true;

	FILE* shader_file = fopen(shader_filename, "r");

	if (shader_file == nullptr)
	{
		succeeded = false;
	}
	else
	{
		fseek(shader_file, 0, SEEK_END);
		int shader_file_length = ftell(shader_file);
		fseek(shader_file, 0, SEEK_SET);

		char* shader_source = new char[shader_file_length];

		shader_file_length = fread(shader_source, 1, shader_file_length, shader_file);

		unsigned int shader_handle = glCreateShader(shader_type);

		glShaderSource(shader_handle, 1, &shader_source, &shader_file_length);
		glCompileShader(shader_handle);

		int success = GL_FALSE;
		//int log_length = 0;
		glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint log_length = 0;

			glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_length);
			char* infoLog = new char[log_length];

			glGetShaderInfoLog(shader_handle, log_length, NULL, infoLog);
			printf("ERROR: Failure to compile the Shader program.\n");
			printf("%s\n", infoLog);

			delete[] infoLog;

			succeeded = false;
		}
		if (succeeded)
		{
			*output = shader_handle;
		}
		

		delete[] shader_source;
		fclose(shader_file);
	}

	return succeeded;
}


bool LoadShaders(
	char* vertex_filename,
	char* geometry_filename,
	char* fragment_filename,
	GLuint* result)
{
	printf("--- SHADERS ---\n");

	bool succeeded = true;

	*result = glCreateProgram();

	//Vertex Shader
	unsigned int vertex_shader;

	if (LoadShaderType(vertex_filename, GL_VERTEX_SHADER, &vertex_shader))
	{
		glAttachShader(*result, vertex_shader);
		glDeleteShader(vertex_shader);

		printf("+ Vertex Shader Loaded Successfully.\n");
	}
	else
		printf("- Vertex Shader Failed to Load.\n");


	//Geometry Shader
	if (geometry_filename != nullptr)
	{
		unsigned int geometry_shader;
		if (LoadShaderType(geometry_filename, GL_GEOMETRY_SHADER, &geometry_shader))
		{
			glAttachShader(*result, geometry_shader);
			glDeleteShader(geometry_shader);

			printf("+ Geometry Shader Loaded Successfully.\n");
		}
		else
		{
			printf("- Geometry Shader Failed to Load.\n");
		}

	}
	else
		printf("No Geometry Shader Assigned to Load.\n");


	//Fragment Shader
	if (fragment_filename != nullptr)
	{
		unsigned int fragment_shader;
		if (LoadShaderType(fragment_filename, GL_FRAGMENT_SHADER, &fragment_shader))
		{
			
			glAttachShader(*result, fragment_shader);
			glDeleteShader(fragment_shader);

			printf("+ Fragment Shader Loaded Successfully.\n");
		}
		else
		{
			printf("- Fragment Shader Failed to Load.\n");
		}
	}
	else
		printf("No Fragment Shader to Load.\n");

	//Link Program
	glLinkProgram(*result);


	//Check if successful
	GLint success;
	glGetProgramiv(*result, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint log_length;

		glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &log_length);
		char* infoLog = new char[log_length];

		glGetProgramInfoLog(*result, log_length, 0, infoLog);
		printf("ERROR: Failure to link to the Shader program.\n");
		printf("%s\n", infoLog);

		delete[] infoLog;

		succeeded = false;
	}

	return succeeded;
}


OpenGLData GenerateQuad(float size)
{
	OpenGLData quad;

	VertexNormal vertex_data[4];

	vertex_data[0].position = vec4(-size, 0, -size, 1);
	vertex_data[1].position = vec4(-size, 0, size, 1);
	vertex_data[2].position = vec4(size, 0, size, 1);
	vertex_data[3].position = vec4(size, 0, -size, 1);

	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);

	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);

	vertex_data[0].tex_coord = vec2(0, 0);
	vertex_data[1].tex_coord = vec2(0, 1);
	vertex_data[2].tex_coord = vec2(1, 1);
	vertex_data[3].tex_coord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	quad.m_index_count = 6;

	glGenVertexArrays(1, &quad.m_VAO);
	glGenBuffers(1, &quad.m_VBO);
	glGenBuffers(1, &quad.m_IBO);

	glBindVertexArray(quad.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, quad.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//normal
	glEnableVertexAttribArray(2);	//tangent
	glEnableVertexAttribArray(3);	//tex coord

	//Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(VertexNormal), 0);

	//Normal
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
		sizeof(VertexNormal), (void*)(sizeof(vec4)* 1));

	//Tangent
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,
		sizeof(VertexNormal), (void*)(sizeof(vec4)* 2));

	//Tex Coord
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexNormal), (void*)(sizeof(vec4)* 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return quad;
}



//LoadTextures(const char* a_diffuse_file,
//	const char* a_normal_file,
//	const char* a_specular_file)
//{
//
//	int width = 0;
//	int height = 0;
//	int channels;
//
//	//DIFFUSE
//	unsigned char* data = stbi_load(a_diffuse_file,
//		&width, &height,
//		&channels, STBI_default);
//
//	glGenTextures(1, &m_diffuse_texture);
//	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);
//
//	//,type of channel, actual data providing it (image)
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
//		GL_RGB, GL_UNSIGNED_BYTE, data);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	stbi_image_free(data);
//
//	////////
//
//
//	//NORMAL
//	data = stbi_load(a_normal_file,
//		&width, &height,
//		&channels, STBI_default);
//
//	glGenTextures(1, &m_normal_texture);
//	glBindTexture(GL_TEXTURE_2D, m_normal_texture);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
//		GL_RGB, GL_UNSIGNED_BYTE, data);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	stbi_image_free(data);
//
//	////////
//
//
//	//SPECULAR
//	data = stbi_load(a_specular_file,
//		&width, &height,
//		&channels, STBI_default);
//
//	glGenTextures(1, &m_specular_texture);
//	glBindTexture(GL_TEXTURE_2D, m_specular_texture);
//
//	//,type of channel, actual data providing it (image)
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
//		GL_RGB, GL_UNSIGNED_BYTE, data);
//
//	//Texture Filtering Options			(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST_MIPMAP_NEAREST)
//	//What to use when pixels are too big 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//What to use when pixels are too small
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	stbi_image_free(data);
//
//	////////
//}