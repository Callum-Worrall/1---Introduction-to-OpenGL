#include "SolarSystemTest.h"

//SolarSystemTest::SolarSystemTest() : Application()
//{
//	m_window_width = 1280.0f;
//	m_window_height = 720.0f;
//}
//
//
//SolarSystemTest::~SolarSystemTest()
//{
//	Application::~Application();
//}


bool SolarSystemTest::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Start up Gizmos
	Gizmos::create();

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);

	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(15);

	//Set m_timer to Zero
	m_timer = 0.0f;

	//Set Colours
	white = vec4(1);
	black = vec4(0, 0, 0, 1);
	red = vec4(1, 0, 0, 1);
	blue = vec4(0, 0, 1, 1);
	yellow = vec4(1, 1, 0, 1);

	return true;
}


bool SolarSystemTest::ShutDown()
{
	delete m_camera;

	//Good practice for memory efficiency
	Gizmos::destroy();

	Application::ShutDown();

	return true;
}


bool SolarSystemTest::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	//dynamically cast time as a float and get
	float DeltaTime = (float)glfwGetTime();

	//reset time to 0
	glfwSetTime(0.0f);

	m_timer += DeltaTime;

	//Camera Update
	m_camera->Update(DeltaTime);

	////create circular rotation for camera
	//m_camera_x = sinf(m_timer) * 10;
	//m_camera_z = cosf(m_timer) * 10;


	////Set Camera Position, Rotation (Yaw, Pitch) , Something
	//mat4 m_view = glm::lookAt(vec3(m_camera_x, 10, m_camera_z), vec3(0, 0, 0), vec3(0, 1, 0));


	//	Put Base Code Here	//


	//						//
	

	Gizmos::clear();

	////Creates Transform Axis
	Gizmos::addTransform(mat4(1));

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



	//Draw Grid 20 x 20
	for (int i = 0; i <= 20; i++)
	{
		//x == -10 + i
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
			i == 10 ? white : black);
	}

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



	glfwSwapBuffers(m_window);

	glfwPollEvents();

	return true;
}


bool SolarSystemTest::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	//Draws Transform Axis
	//Gizmos::draw(m_projection, m_view);

	////Draw Grid 20 x 20
	//for (int i = 0; i <= 20; i++)
	//{
	//	//x == -10 + i
	//	Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
	//		i == 10 ? white : black);
	//
	//	Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
	//		i == 10 ? white : black);
	//
	//}
	//
	////Sun Object Mesh
	//Gizmos::addSphere(mat4_Sun[3].xyz,
	//	2.0f, 20, 20, yellow, &mat4_Sun);
	//
	////Planet 1 Object Mesh
	//Gizmos::addSphere(mat4_Planet1[3].xyz,
	//	0.2f, 20, 20, blue, &mat4_Planet1);
	//
	////Planet 2 Object Mesh
	//Gizmos::addSphere(mat4_Planet2[3].xyz,
	//	0.27f, 20, 20, blue, &mat4_Planet2);
	//
	////Planet 3 Object Mesh
	//Gizmos::addSphere(mat4_Planet3[3].xyz,
	//	0.6f, 20, 20, blue, &mat4_Planet3);
	//
	////Planet 2 Moon 1 Object Mesh
	//Gizmos::addSphere(mat4_Planet2Moon1[3].xyz,
	//	0.07f, 20, 20, red, &mat4_Planet2Moon1);



	return true;
}


mat4 SolarSystemTest::BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation)
{
	mat4 result = glm::rotate(local_rotation, vec3(0, 1, 0)) *
		glm::translate(vec3(radius, 0, 0)) *
		glm::rotate(orbit_rotation, vec3(0, 1, 0));

	return result;
}
