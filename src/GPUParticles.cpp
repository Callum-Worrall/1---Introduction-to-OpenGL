#include "GPUParticles.h"

GPUParticles::GPUParticles()
{

}

GPUParticles::~GPUParticles()
{

}


bool GPUParticles::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}
	m_Timer = 0;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	/////////////////////


	m_emitter = new GPUPointEmitter();
	m_emitter->Initialize(
		100000,
		0.1f, 10.0f,
		5, 20,
		1, 0.1f,
		glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));


	m_emitterControl = new GUI();
	m_emitterControl->StartUp("Emitter Controls", 1280, 720, m_window);

	TwAddVarRW(m_emitterControl->GetBar(), "Background Colour", TW_TYPE_COLOR4F,
		&m_background_color, "");

	//TwAddVarRW(m_emitterControl->GetBar(), "Rate", TW_TYPE_FLOAT,
	//	&m_emitter->m_emitRate, "group = Particle");
	//
	//TwAddVarRW(m_emitterControl->GetBar(), "Timer", TW_TYPE_FLOAT,
	//	&m_emitter->m_emitTimer, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Lifespan Min", TW_TYPE_FLOAT,
		&m_emitter->m_lifespanMin, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Lifespan Max", TW_TYPE_FLOAT,
		&m_emitter->m_lifespanMax, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "Start Colour", TW_TYPE_COLOR4F,
		&m_emitter->m_startColour, "group = Particle");

	TwAddVarRW(m_emitterControl->GetBar(), "End Colour", TW_TYPE_COLOR4F,
		&m_emitter->m_endColour, "group = Particle");


	/////////////////////

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(15);

	return true;
}


bool GPUParticles::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool GPUParticles::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();
	m_Timer += DeltaTime;
	//Set time to 0
	glfwSetTime(0.0f);

	//Camera Update
	m_camera->Update(DeltaTime);

	/////////////////////

	glClearColor(m_background_color.x,
		m_background_color.y,
		m_background_color.z,
		m_background_color.w);







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

bool GPUParticles::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////

	m_emitter->Draw(m_Timer,
		m_camera->GetWorldTransform(),
		m_camera->GetProjectionView());


	m_emitterControl->Draw();

	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}
