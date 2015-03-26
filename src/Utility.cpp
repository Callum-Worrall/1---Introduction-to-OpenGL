#include "Utility.h"
#include "glm_include.h"
#include <cstdio>
#include "gl_core_4_4.h"


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


