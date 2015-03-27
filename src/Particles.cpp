#include "Particles.h"
#include "Utility.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include "stb_image.h"


bool Particles::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	/////////////////////


	m_emitter = new Emitter();

	LoadShaders(
		"./shaders/particle_vertex.glsl",
		nullptr,
		"./shaders/particle_fragment.glsl",
		&m_program_ID);

	m_emitter->Initialize(
		6000, //MAX
		vec3(0, 0, 0), //POSITION
		500, //RATE
		1, 1.1, //LIFESPAN (min, max)
		2.5f, 5.0f, //VELOCITY (min, max)
		1, 0.05f, //SIZE (start, end)
		vec4(1, 0, 0, 1), vec4(1, 1.4f, 0, 1) //COLOUR (start, end)
	);

	m_emitterControl = new GUI();
	m_emitterControl->StartUp("Emitter Controls", 1280, 720);
	//TwAddSeparator(m_emitterControl->GetBar(), "LIGHT BAR", "");
	TwAddVarRW(m_emitterControl->GetBar(), "Background Colour", TW_TYPE_COLOR4F,
		&m_background_color, "");
	
	TwAddVarRW(m_emitterControl->GetBar(), "Rate", TW_TYPE_FLOAT,
		&m_emitter->m_emitRate, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Timer", TW_TYPE_FLOAT,
		&m_emitter->m_emitTimer, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Lifespan Min", TW_TYPE_FLOAT,
		&m_emitter->m_lifespanMin, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Lifespan Max", TW_TYPE_FLOAT,
		&m_emitter->m_lifespanMax, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Start Colour", TW_TYPE_COLOR4F,
		&m_emitter->m_startColor, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "End Colour", TW_TYPE_COLOR4F,
		&m_emitter->m_endColor, "group = Particle");

	/////////////////////

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(10);

	return true;
}


bool Particles::ShutDown()
{
	delete m_emitter;
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool Particles::Update()
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

	/////////////////////

	glClearColor(m_background_color.x,
		m_background_color.y,
		m_background_color.z,
		m_background_color.w);


	//m_emitter->Update(DeltaTime, m_camera->GetWorldTransform());

	m_emitter->Update(DeltaTime);
	m_emitter->UpdateVertexData(
		m_camera->GetWorldTransform()[3].xyz,
		m_camera->GetWorldTransform()[1].xyz);



	/////////////////////

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

bool Particles::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////



	glUseProgram(m_program_ID);

	int proj_view_uniform = glGetUniformLocation(m_program_ID, "projection_view");

	glUniformMatrix4fv(proj_view_uniform, 1, GL_FALSE,
		(float*)&m_camera->GetProjectionView());

	m_emitter->Render();


	m_emitterControl->Draw();


	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

