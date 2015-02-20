#include "Texturing.h"
#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
//#include "Vertex.h"

#include "Utility.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool Texturing::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	////Load Texture Function
	//LoadTexture("./textures/crate.png");
	//
	//GenerateQuad(5.0f);

	GenerateSphere(10, 20, 20);

	Utility::LoadShader("./shaders/texture_vertex.glsl",
						"./shaders/texture_fragment.glsl",
							&m_program_ID /* &m_texture */);


	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);

	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(3);


	return true;
}

bool Texturing::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool Texturing::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();

	//Set time to 0
	glfwSetTime(0.0f);

	//Camera Update
	m_camera->Update(DeltaTime);

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

bool Texturing::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program_ID);

	int view_proj_uniform = glGetUniformLocation(m_program_ID, "projection_view");

	glUniformMatrix4fv(view_proj_uniform, 1, GL_FALSE, (float*)&m_camera->GetProjectionView() );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	int diffuse_location = glGetUniformLocation(m_program_ID, "diffuse");

	glUniform1i(diffuse_location, 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



	Gizmos::draw(m_camera->GetProjectionView());

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void Texturing::LoadTexture(const char* filename)
{
	int width = 0;	int height = 0;	int channels;

	unsigned char* data = stbi_load(filename,
									&width, &height,
									&channels, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//,type of channel, actual data providing it (image)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//Texture Filtering Options			(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST_MIPMAP_NEAREST)
	//What to use when pixels are too big 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//What to use when pixels are too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	stbi_image_free(data);
}

void Texturing::GenerateQuad(float size)
{
	VertexTexCoord vertex_data[4];

	vertex_data[0].position = vec4(-size, 0, -size, 1);
	vertex_data[1].position = vec4(-size, 0, size, 1);
	vertex_data[2].position = vec4(size, 0, size, 1);
	vertex_data[3].position = vec4(size, 0, -size, 1);

	vertex_data[0].tex_coord = vec2(0, 0);
	vertex_data[1].tex_coord = vec2(0, 1);
	vertex_data[2].tex_coord = vec2(1, 1);
	vertex_data[3].tex_coord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };


	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexCoord)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//tex coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
								sizeof(VertexTexCoord), 0); //position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
								sizeof(VertexTexCoord), (void*)sizeof(vec4)); //tex coord

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


OpenGLData Texturing::GenerateSphere(float radius, int rows, int cols)
{
	OpenGLData result = {};

	VertexTexCoord* verts = new VertexTexCoord[cols + 1];

	verts[0].position = vec4(0, 0, 0, 1);
	verts[0].tex_coord = vec2(0.5, 0.5);

	//Set Centre Vertex
	verts[0].position = vec4(0);

	//Assign other Vertices
	for (int i = 0; i < cols; i++)
	{
		verts[i + 1].position = vec4(sinf((i / (float)cols) * 2 * 3.14159),
							0,
							cosf((i / (float)cols) * 2 * 3.14159),
							1);

		verts[i + 1].tex_coord = vec2(verts[i + 1].position.x + 0.5f,
										verts[i + 1].position.z + 0.5f);
	}

	unsigned int* indices = new unsigned int[3 * cols];

	for (int i = 0; i < cols; i++)
	{
		indices[i * 3 + 0] = 0;
		indices[i * 3 + 1] = i + 1;
		indices[i * 3 + 2] = ((i + 1) % cols) + 1;
	}

	indices[3 * cols - 1] = 1; //unsigned int[3 * cols)

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexCoord)* (cols + 1),
		verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (cols * 3),
		indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//tex coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(VertexTexCoord), 0); //position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexTexCoord), (void*)sizeof(vec4)); //tex coord

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] verts;
	delete[] indices;

	return result;
}