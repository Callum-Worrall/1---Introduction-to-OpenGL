#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
	SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
}


FlyCamera::~FlyCamera()
{

}


FlyCamera::FlyCamera(float width, float height, float a_speed, float a_sensitivity)
{
	m_speed = a_speed;
	m_sensitivity = a_sensitivity;
}

void FlyCamera::SetPerspective(float a_fieldOfView, float aspectRatio, float a_near, float a_far)
{
	projectionTransform = glm::perspective(a_fieldOfView, aspectRatio, a_near, a_far);
}


bool FlyCamera::Update(float deltaTime)
{
	mat4 world = Camera::GetWorldTransform();

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
	{
		world[3] -= world[2] * m_speed * deltaTime;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
	{
		world[3] += world[2] * m_speed * deltaTime;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
	{
		world[3] -= world[0] * m_speed * deltaTime;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
	{
		world[3] += world[0] * m_speed * deltaTime;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		world[3] -= world[1] * m_speed * deltaTime;
	}

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
	{
		world[3] += world[1] * m_speed * deltaTime;
	}


	GLFWwindow* curr_window = glfwGetCurrentContext();


	if (glfwGetMouseButton(curr_window, 1))
	{
		double x_delta, y_delta;

		glfwSetInputMode(curr_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		//glfwSetCursorPos(curr_window, GLFW_CURSOR)

		glfwGetCursorPos(curr_window, &x_delta, &y_delta);
		glfwSetCursorPos(curr_window, 1280.0f / 2.0f, 720.0f / 2.0f);

		x_delta -= 1280.0f / 2;
		y_delta -= 720.0f / 2;

		x_delta /= 1280.0f / 2;
		y_delta /= 720.0f / 2;

		x_delta *= -m_sensitivity;
		y_delta *= -m_sensitivity;


		vec3 camera_right = vec3(world[0]);
		
		mat4 yaw = glm::rotate((float) x_delta, vec3(0, 1, 0));
		mat4 pitch = glm::rotate((float)y_delta, camera_right);

		mat4 rot = yaw * pitch;

		world[0] = rot * world[0];
		world[1] = rot * world[1];
		world[2] = rot * world[2];
	}
	else
	{
		glfwSetInputMode(curr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	world[3][3] = 1;

	viewTransform = glm::inverse(world);
	worldTransform = world;
	Camera::UpdateProjectionViewTransform();

	return true;
}


void FlyCamera::SetSpeed(float a_speed)
{
	m_speed = a_speed;
}

void FlyCamera::SetSensitivity(float a_sensitivity)
{
	m_sensitivity = a_sensitivity;
}