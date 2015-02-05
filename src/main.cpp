#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <cstdio>

#include "Gizmos.h"

#define GLM_SWIZZLE

#include "glm/glm.hpp"
#include "glm/ext.hpp"

using glm::vec1;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;


mat4 BuildOrbitMatrix(float local_rotation, float radius, float orbit_rotation)
{
	mat4 result = glm::rotate(local_rotation, vec3(0, 1, 0)) *
					glm::translate(vec3(radius, 0, 0)) *
						glm::rotate(orbit_rotation, vec3(0, 1, 0));

	return result;
}

int main()
{

	//if broken, return -1
	if (glfwInit() == false)
		return -1;

	//GLFWmonitor** monitors = glfwGetMon

	//create window x, y, title, monitor, share?
	GLFWwindow* window = glfwCreateWindow(1290, 720, "Computer Graphics - Ex Nihilo", nullptr, nullptr);

	//Check that the window is alright to use
	if (window == nullptr)
	{
		return -2;
	}

	//Make the Current Context the GLFW window
	glfwMakeContextCurrent(window);

	ogl_LoadFunctions();

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
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
	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0,0,0), vec3(0, 1, 0));

	//glm::perspective - Sets Field of View (Field of View Y, Aspect Ratio, Near, Far)
	mat4 projection = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);

	//
	float camera_x = -10.0f;
	float camera_z = -10.0f;

	float timer = 0.0f;

	while (glfwWindowShouldClose(window) == false)
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


		//le cool code
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();
		
		//Creates Identity Matrix
		Gizmos::addTransform(mat4(1));

		//Create White and Black Colours
		vec4 white(1);
		vec4 black(0, 0, 0, 1);
		vec4 red(1, 0, 0, 1);
		vec4 blue(0, 0, 1, 1);
		vec4 yellow(1, 1, 0, 1);

		//Draw 20 lines to the screen
		for (int i = 0; i <= 20; i++)
		{
			//x == -10 + i
			Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
				i == 10 ? white : black);

			Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
				i == 10 ? white : black);

			//Create Triangle
			//Gizmos::addTri(vec3(0, 1, 0), vec3(10, 5, 0), vec3(10, 1, 0), vec4(68.5f, 0.5f, 0.8, 1.0f));

			//Create Cube
			//Gizmos::addAABB(vec3(5, 5, 5), vec3(1,1,1), vec4(0.0f, 0.0f, 90.0f, 1.0f));
			//
			//Gizmos::addAABBFilled(vec3(-5, 5, -5), vec3(1, 1, 1), vec4(0.0f, 0.0f, 90.0f, 1.0f));
		}

		//Sun's Rotation Matrix
		mat4 sun_transform = BuildOrbitMatrix(timer, 0, 0);
		//Sun
		Gizmos::addSphere(sun_transform[3].xyz,
							2.0f, 20, 20, yellow, &sun_transform);

		//Planet 1's Orbit (Rotation Matrix)
		mat4 planet1_transform = BuildOrbitMatrix(timer * 0.5f, 7, 0);
		//Planet 1
		Gizmos::addSphere(planet1_transform[3].xyz,
			0.5f, 20, 20, blue, &planet1_transform);

		//Planet 1 Moon 1's Orbit (Rotation Matrix)
		mat4 planet1_moon1_transform = planet1_transform * glm::translate(vec3(2, 0, 0));

		mat4 planet1_moon1_transform = BuildOrbitMatrix(timer * 0.5f, 7, 0);

		//Planet 1 Moon 1's Orbit
		Gizmos::addSphere(planet1_moon1_transform[3].xyz,
							0.3f, 20, 20, red, &planet1_moon1_transform);

		//Planet 2's Orbit (Rotation Matrix)
		mat4 planet2_transform = sun_transform *
			glm::translate(vec3(3, 0, 0)) *
			glm::rotate(timer * 1.5f, vec3(0, 1, 0));
		//Planet 2
		Gizmos::addSphere(planet2_transform[3].xyz,
			0.3f, 20, 20, blue, &planet2_transform);



		//Draws Gyroscope
		Gizmos::draw(projection, view);

		//
		glfwSwapBuffers(window);
		//
		glfwPollEvents();
	}

	//Good practice for memory efficiency
	Gizmos::destroy();

	//
	glfwDestroyWindow(window);

	//
	glfwTerminate();
	return 0;
}