#include "SceneManagement.h"


bool SceneManagement::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	gridActive = false;

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(15);

	m_gPressed = false;


	/////////////////////

	//Boundary Plane
	plane = vec4(0.0f, 1, 0, -1);	// ax + by + cz + d = 0
									// 0 + 1 + 0 + -1 = 0

	//Sphere
	sphere.centre = vec3(0, cosf((float)glfwGetTime()) + 1, 0);
	sphere.radius = 0.5f;


	//Frustrum Planes
	GetFrustumPlanes(m_camera->GetProjectionView(), frustumPlanes);


	/////////////////////

	return true;
}


bool SceneManagement::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool SceneManagement::Update()
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

	Gizmos::clear();

	Input();

	Gizmos::addSphere(
		sphere.centre,		//centre
		sphere.radius,		//radius
		8, 8,				//rows, cols
		vec4(1, 0, 1, 1)	//colour
		);


	float d = dot(vec3(plane), sphere.centre) + plane.w;	//dot product

	////Bounding Sphere Check
	//if (d > sphere.radius)
	//	printf("Front\n");
	//else if (d < -sphere.radius)
	//	printf("Back\n");
	//else
	//	printf("On the plane\n");	////Plane Check
	//if (d < 0)
	//	printf("Back\n");
	//else if (d > 0)
	//	printf("Front\n");
	//else
	//	printf("On the plane\n");

	//Plane Colour Change
	vec4 planeColour(1, 1, 0, 1);
	if (d > sphere.radius)
		planeColour = vec4(0, 1, 0, 1);
	else if (d < -sphere.radius)
		planeColour = vec4(1, 0, 0, 1);

	Gizmos::addTri(
		vec3(4, 1, 4),
		vec3(-4, 1, -4),
		vec3(-4, 1, 4),
		planeColour);

	Gizmos::addTri(
		vec3(4, 1, 4),
		vec3(4, 1, -4),
		vec3(-4, 1, -4),
		planeColour);

	for (int i = 0; i < 6; i++)
	{
		float planeDot = dot(
							vec3(frustumPlanes[i]),
								sphere.centre) +
						 frustumPlanes[i].w;

		if (planeDot < -sphere.radius)
		{
			printf("Behind, don't render it!\n");
			break;
		}
		else if (planeDot < sphere.radius)
		{
			printf("Touching, we still need to render it!\n");
		}
		else
		{
			printf("Front, fully visible so render it!\n");
		}
	}

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

void SceneManagement::GetFrustumPlanes(const mat4 &transform, vec4* planes)
{
	// right side
	planes[0] = vec4(transform[0][3] - transform[1][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);

	// left side
	planes[1] = vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);

	// top
	planes[2] = vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);

	// bottom
	planes[3] = vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);

	// far
	planes[4] = vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);

	// near
	planes[5] = vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]);

	for (int i = 0; i < 6; i++)
	{
		planes[i] = glm::normalize(planes[i]);
	}
}

bool SceneManagement::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////














	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void SceneManagement::Input()
{
	GLFWwindow* window = glfwGetCurrentContext();

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && m_gPressed == false)
	{
		m_gPressed = true;
		if (gridActive == true)
		{
			gridActive = false;
		}
		else
			gridActive = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
	{
		m_gPressed = false;
	}


	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		sphere.centre.y += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		sphere.centre.y -= 1;
	}

}