#include "SolarSystemTest.h"


void BuildCelestialBody()
{

}

mat4 SolarSystemTest::BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation)
{
	mat4 result = glm::rotate(local_rotation, vec3(0, 1, 0)) *
		glm::translate(vec3(radius, 0, 0)) *
		glm::rotate(orbit_rotation, vec3(0, 1, 0));

	return result;
}

SolarSystemTest::SolarSystemTest() : Application()
{
}


SolarSystemTest::~SolarSystemTest()
{
	Application::~Application();
}

bool SolarSystemTest::Create(float screen_width, float screen_height, const char* window_title)
{
	//Open Graphics Library Frame Work Initialized
	if (glfwInit() == false)
		return false;

	//create window x, y, title, monitor, share?
	window = glfwCreateWindow(screen_width, screen_height, window_title, nullptr, nullptr);

	//Check that the window is alright to use
	if (window == nullptr)
	{
		return false;
	}

	//Make the Current Context the GLFW window
	glfwMakeContextCurrent(window);

	ogl_LoadFunctions();

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	//Print Version Name to Console
	int major_version = ogl_GetMajorVersion();
	int minor_version = ogl_GetMinorVersion();
	printf("Successfully loaded OpenGL version %d.%d\n", major_version, minor_version);


	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//Start up Gizmos
	Gizmos::create();

	//camera - position, rotation, up
	view = glm::lookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));

	//glm::perspective - Sets Field of View (Field of View Y, Aspect Ratio, Near, Far)
	projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	//Camera Co-Ordinates
	camera_x = -10.0f;
	camera_z = -10.0f;

	//Set Timer to Zero
	timer = 0.0f;

	//Create Colours
	white = vec4(1);
	black = vec4(0, 0, 0, 1);
	red = vec4(1, 0, 0, 1);
	blue = vec4(0, 0, 1, 1);
	yellow = vec4(1, 1, 0, 1);

	return true;
}

bool SolarSystemTest::StartUp()
{
	return true;
}


bool SolarSystemTest::ShutDown()
{
	//Good practice for memory efficiency
	Gizmos::destroy();

	//
	glfwDestroyWindow(window);

	//
	glfwTerminate();
	return true;
}


bool SolarSystemTest::Update()
{
	//dynamically cast time as a float and get
	float dt = (float)glfwGetTime();

	//reset time to 0
	glfwSetTime(0.0f);

	timer += dt;

	////create circle for rotation
	//camera_x = sinf(timer) * 10;
	//camera_z = cosf(timer) * 10;


	//Set Camera Position, Rotation (Yaw, Pitch) , Something
	mat4 view = glm::lookAt(vec3(camera_x, 10, camera_z), vec3(0, 0, 0), vec3(0, 1, 0));


	//	Put Base Code Here	//


	//						//
	

	Gizmos::clear();

	////Creates Identity Matrix
	Gizmos::addTransform(mat4(1));

	//Sun Orbit (Transform Matrix)
	mat4_Sun = BuildOrbitMatrix(timer, 0, 0);

	//Planet 1 Orbit (Transform Matrix)
	mat4_Planet1 = mat4_Sun * BuildOrbitMatrix(timer * 2.5f, 4, 0);

	//Planet 2 Orbit (Transform Matrix)
	mat4_Planet2 = mat4_Sun * BuildOrbitMatrix(timer * 0.5f, 9, 0);
	
	//Planet 2 Moon 1 Orbit (Transform Matrix)
	mat4_Planet2Moon1 = mat4_Planet2 * BuildOrbitMatrix(timer * 2.5f, 1, 5);

	//Planet 3 Orbit (Transform Matrix)
	mat4_Planet3 = mat4_Sun * BuildOrbitMatrix(timer * 0.5f, 13, 0);



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



	glfwSwapBuffers(window);

	glfwPollEvents();

	return true;
}


bool SolarSystemTest::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draws Transform Axis
	Gizmos::draw(projection, view);

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
