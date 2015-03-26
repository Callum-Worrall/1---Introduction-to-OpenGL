#include "PostProcess.h"
#include "Utility.h"

bool PostProcess::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	gridActive = false;

	Gizmos::create();

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(15);

	gPressed = true;

	/////////////////////

	//Set m_timer to Zero
	m_timer = 0.0f;

	//Create the Quad Mesh
	GenerateScreenSpaceQuad();

	//Build Frame Buffer
	GenerateFramebuffer();

	//Load the post process shaders
	LoadShaders("./shaders/post_vertex.glsl", nullptr, "./shaders/post_fragment.glsl", &m_post_program_id);

	//Set Colours
	white = vec4(1);
	black = vec4(0, 0, 0, 1);
	red = vec4(1, 0, 0, 1);
	blue = vec4(0, 0, 1, 1);
	yellow = vec4(1, 1, 0, 1);

	/////////////////////

	return true;
}


bool PostProcess::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool PostProcess::Update()
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

	Input();

	/////////////////////


	////Creates Transform Axis
	Gizmos::addTransform(mat4(1));

	//Set the orbits

	//Sun Orbit (Transform Matrix)
	mat4_Sun = BuildOrbitMatrix(m_timer, 0, 0);

	//Planet 1 Orbit (Transform Matrix)
	mat4_Planet1 = mat4_Sun * BuildOrbitMatrix(m_timer * 2.5f, 4, 0);

	//Planet 2 Orbit (Transform Matrix)
	mat4_Planet2 = mat4_Sun * BuildOrbitMatrix(m_timer * 0.5f, 9, 0);

	//Planet 2 Moon 1 Orbit (Transform Matrix)
	mat4_Planet2Moon1 = mat4_Planet2 * BuildOrbitMatrix(m_timer * 2.5f, 1, 5);

	//Planet 3 Orbit (Transform Matrix)
	mat4_Planet3 = mat4_Sun * BuildOrbitMatrix(m_timer * 0.5f, 13, 0);


	//Create the Meshes

	//Sun Object Mesh
	Gizmos::addSphere(mat4_Sun[3].xyz,
		2.0f, 20, 20, yellow, &mat4_Sun);

	//Planet 1 Object Mesh
	Gizmos::addSphere(mat4_Planet1[3].xyz,
		0.2f, 20, 20, blue, &mat4_Planet1);

	//Planet 2 Object Mesh
	Gizmos::addSphere(mat4_Planet2[3].xyz,
		0.27f, 20, 20, blue, &mat4_Planet2);

	//Planet 3 Object Mesh
	Gizmos::addSphere(mat4_Planet3[3].xyz,
		0.6f, 20, 20, blue, &mat4_Planet3);

	//Planet 2 Moon 1 Object Mesh
	Gizmos::addSphere(mat4_Planet2Moon1[3].xyz,
		0.07f, 20, 20, red, &mat4_Planet2Moon1);

	/////////////////////

	vec4 white(1);
	vec4 black(0, 0, 0, 1);


	//Draw Grid 20 x 20
	if (gridActive == true)
	{
		for (int i = 0; i <= 20; i++)
		{
			//x == -10 + i
			Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
				i == 10 ? white : black);

			Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
				i == 10 ? white : black);
		}
	}

	return true;
}

bool PostProcess::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_post_program_id);

	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glBindVertexArray(m_quad.m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}


void PostProcess::GenerateFramebuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_fbo_texture);
	glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fbo_texture, 0);

	glGenRenderbuffers(1, &m_fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fbo_depth);

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FRAMEBUFFER DID NOT GENERATE PROPERLY!!\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcess::GenerateScreenSpaceQuad()
{
	vec2 half_texel = 1.0f / vec2(1280, 720) * 0.5f;

	float vertex_data[]
	{
		-1, -1, 0, 1, half_texel.x, half_texel.y,
		1, -1, 0, 1, 1 - half_texel.x, half_texel.y,
		1, 1, 0, 1, 1 - half_texel.x, 1 - half_texel.y,
		-1, 1, 0, 1, half_texel.x, 1 - half_texel.y,
	};

	unsigned int index_data[] =
	{
		0, 1, 2, 0, 2, 3
	};

	glGenVertexArrays(1, &m_quad.m_VAO);

	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);

	glBindVertexArray(m_quad.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)* 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)* 6, (void*)(sizeof(float)* 4));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void PostProcess::Input()
{
	GLFWwindow* window = glfwGetCurrentContext();

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && gPressed == false)
	{
		if (gridActive == true)
		{
			gridActive = false;
		}
		else
			gridActive = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
	{
		gPressed = false;
	}
}


mat4 PostProcess::BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation)
{
	mat4 result = glm::rotate(local_rotation, vec3(0, 1, 0)) *
		glm::translate(vec3(radius, 0, 0)) *
		glm::rotate(orbit_rotation, vec3(0, 1, 0));

	return result;
}
