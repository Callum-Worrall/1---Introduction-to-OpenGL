#include "RenderingGeometry.h"
#include "Vertex.h"
#include "Utility.h"


RenderingGeometry::RenderingGeometry()
{
	m_window_width = 1280.0f;
	m_window_height = 720.0f;
}


RenderingGeometry::~RenderingGeometry()
{
}


bool RenderingGeometry::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	GenerateShader();
	GenerateGrid(10, 10);
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);

	//Start up Gizmos
	Gizmos::create();

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f);

	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(10);

	//Set m_timer to Zero
	m_timer = 0.0f;

	return true;
}

bool RenderingGeometry::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool RenderingGeometry::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();

	//Set time to 0
	glfwSetTime(0.0f);

	m_timer += DeltaTime;

	//Camera Update
	m_camera->Update(DeltaTime);

	Gizmos::clear();

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	//Draw Grid 20 x 20
	for (int i = 0; i <= 20; i++)
	{
		//x == -10 + i
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
			i == 10 ? white : black);
	}

	return true;
}

bool RenderingGeometry::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program_ID);

	unsigned int projectionViewUniform = glGetUniformLocation(m_program_ID, "projection_view");
	if (projectionViewUniform >= 0)
	{
		glUniformMatrix4fv(projectionViewUniform, 1, false, (float*)&m_camera->GetProjectionView());
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);

	Gizmos::draw(m_camera->GetProjectionView());

	Application::Draw();

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void RenderingGeometry::GenerateGrid(unsigned int rows, unsigned int cols)
{
	Vertex* vertex_array = new Vertex[(rows + 1) * (cols + 1)];

	for (unsigned int r = 0; r < rows + 1; ++r)
	{
		for (unsigned int c = 0; c < cols + 1; ++c)
		{
			vec4 pos = vec4((float)c, 0, (float)r, 1);
			vertex_array[r * (cols + 1) + c].position = pos;

			vec4 color = vec4((float)c / (cols + 1), 0, (float)r / (rows + 1), 1);
			vertex_array[r * (cols + 1) + c].color = color;
		}
	}
	
	m_index_count = rows * cols * 6;

	unsigned int* index_array = new unsigned int[m_index_count];

	int index_location = 0;
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			//Triangle 1
			index_array[index_location++] = r * (cols + 1) + c;					//2 * (10 + 1) + 5 = 27
			index_array[index_location++] = (r + 1) * (cols + 1) + c;			//(2 + 1) * (10 + 1) + 5 = 38
			index_array[index_location++] = r * (cols + 1) + (c + 1);			//(2 * 
									   
			//Triangle 2										
			index_array[index_location++] = r * (cols + 1) + (c + 1);			
			index_array[index_location++] = (r + 1) * (cols + 1) + c;			
			index_array[index_location++] = (r + 1) * (cols + 1) + (c + 1);	
		}
	}

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, (cols + 1) * (rows + 1) * sizeof(Vertex),
														vertex_array, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Color

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec4));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_count * sizeof(unsigned int), index_array, GL_STATIC_DRAW);

	//Unbind 
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
	delete[] vertex_array;
	delete[] index_array;
}


void RenderingGeometry::GenerateShader()
{
//Create Shaders

	//Vertex Shader
	char* vs_source =	"#version 410\n \
							layout(location=0) in vec4 position;\n \
							layout(location=1) in vec4 color;\n \
							out vec4 out_color;\n \
							uniform mat4 projection_view; \
							void main() \n \
							{ \
								out_color = color; \n \
								vec4 pos = position; \n \
								\
								gl_Position = projection_view * position; \
							} \n";
	
	//Fragment Shader
	char* fs_source =	"#version 410 \n \
							in vec4 out_color; \n \
							out vec4 frag_color; \n \
							void main() \n \
							{ \n \
							frag_color = out_color; \n \
							} \n";

	if (Utility::LoadShader(vs_source, fs_source, 0) == true)
	{

	}


	/*unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vertex_shader, 1, &vs_source, 0);
	glCompileShader(vertex_shader);
	
	glShaderSource(fragment_shader, 1, &fs_source, 0);
	glCompileShader(fragment_shader);
	
	m_program_ID = glCreateProgram();
	
	glAttachShader(m_program_ID, vertex_shader);
	glAttachShader(m_program_ID, fragment_shader);
	glLinkProgram(m_program_ID);
	
	int success = GL_FALSE;
	glGetProgramiv(m_program_ID, GL_LINK_STATUS, &success);
	
	if (success == GL_FALSE)
		{
			int log_length = 0;
			glGetProgramiv(m_program_ID, GL_INFO_LOG_LENGTH, &log_length);
			char* infoLog = new char[log_length];
		
			glGetProgramInfoLog(m_program_ID, log_length, 0, infoLog);
			printf("Error: Failed to link shader program!\n");
			printf("%s\n", infoLog);
			delete[] infoLog;
		}*/
}
