#include "Utility.h"

bool Utility::LoadShader(char* vertex_filename, char* fragment_filename, GLuint* result)
{
	bool succeeded = true;

	//result = 0;

	FILE* vertex_file = fopen(vertex_filename, "r");
	FILE* fragment_file = fopen(fragment_filename, "r");

	if (vertex_file == 0 || fragment_file == 0)
	{
		succeeded = false;
	}
	else
	{
		fseek(vertex_file, 0, SEEK_END);
		int vertex_file_length = ftell(vertex_file);
		ftell(vertex_file);

		fseek(fragment_file, 0, SEEK_END);
		int fragment_file_length = ftell(fragment_file);
		ftell(fragment_file);

		char* vs_source = new char[vertex_file_length];
		char* fs_source = new char[fragment_file_length];

		fread(vs_source, 1, vertex_file_length, vertex_file);
		fread(fs_source, 1, fragment_file_length, fragment_file);

		int success = GL_FALSE;
		int log_length = 0;

		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vs_source, 0);
		glCompileShader(vertex_shader);

		glShaderSource(fragment_shader, 1, &fs_source, 0);
		glCompileShader(fragment_shader);

		*result = glCreateProgram();

		glAttachShader(*result, vertex_shader);
		glAttachShader(*result, fragment_shader);
		glLinkProgram(*result);

		//int success = GL_FALSE;
		glGetProgramiv(*result, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			//int log_length = 0;
			glGetProgramiv(*result, GL_INFO_LOG_LENGTH, &log_length);
			char* infoLog = new char[log_length];

			glGetProgramInfoLog(*result, log_length, 0, infoLog);
			printf("ERROR: Failure to link to the Shader program.\n");
			printf("%s\n", infoLog);

			delete[] infoLog;

			succeeded = false;
		}

		delete[] vs_source;
		delete[] fs_source;

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		fclose;
	}

	return succeeded;
}