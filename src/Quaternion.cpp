#include "Quaternion.h"


bool Quaternion::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	/////////////////

	//identity matrix of a quaternion (no axis, no rotation)

	//all... um... radians apparently... sigh im so dumb

	glm::quat boring_quaternion(1, 0, 0, 0);

	glm::quat euler_quaternion(vec3(3, 5, 7));

	glm::quat mixed_quat = glm::slerp(boring_quaternion,
										euler_quaternion, 0.8f);

	m_positions[0] = vec3(10, 5, 10);
	m_positions[1] = vec3(-10, 0, -10);

	m_rotations[0] = glm::quat(vec3(0, -1, 0));
	m_rotations[1] = glm::quat(vec3(0, 1, 0));

	////////////////
	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);

	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(10);

	//First Frame//
	m_hip_frames[0].position = vec3(0, 5, 0);
	m_hip_frames[0].rotation = glm::quat(vec3(-1, 0, 0));

	m_knee_frames[0].position = vec3(0, -2.5, 0);
	m_knee_frames[0].rotation = glm::quat(vec3(-1, 0, 0));

	m_ankle_frames[0].position = vec3(0, -2.5, 0);
	m_ankle_frames[0].rotation = glm::quat(vec3(0, 0, 0));
	///////////////

	//Second Frame//
	m_hip_frames[1].position = vec3(0, 5, 0);
	m_hip_frames[1].rotation = glm::quat(vec3(1, 0, 0));

	m_knee_frames[1].position = vec3(0, -2.5, 0);
	m_knee_frames[1].rotation = glm::quat(vec3(0, 0, 0));

	m_ankle_frames[1].position = vec3(0, -2.5, 0);
	m_ankle_frames[1].rotation = glm::quat(vec3(0, 0, 0));
	///////////////

	return true;
}


mat4 EvaluateKeyframes(Keyframe start, Keyframe end, float t)
{
	vec3 pos = glm::mix(start.position, end.position, t);
	glm::quat rot = glm::slerp(start.rotation, end.rotation, t);

	mat4 result = glm::translate(pos) * glm::toMat4(rot);

	return result;
}


bool Quaternion::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool Quaternion::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();

	Gizmos::clear();

	////////////////

	m_timer += DeltaTime;

	float sin_wave = sinf(m_timer) * 0.5f + 0.5f;



	//Hip, Knee and Ankles Positions & Rotations Evaluated + Cubes Created

	//Evaluation / Interpolation
	hip_bone = EvaluateKeyframes(m_hip_frames[0], m_hip_frames[1], sin_wave);
	knee_bone = EvaluateKeyframes(m_knee_frames[0], m_knee_frames[1], sin_wave);
	ankle_bone = EvaluateKeyframes(m_ankle_frames[0], m_ankle_frames[1], sin_wave);


	mat4 global_hip = hip_bone;
	mat4 global_knee = hip_bone * knee_bone;
	mat4 global_ankle = global_knee * ankle_bone;

	vec3 hip_pos = global_hip[3].xyz;
	vec3 knee_pos = global_knee[3].xyz;
	vec3 ankle_pos = global_ankle[3].xyz;

	Gizmos::addAABBFilled(hip_pos, vec3(0.5f), vec4(1, 1, 0, 1), &global_hip);
	Gizmos::addAABBFilled(knee_pos, vec3(0.5f), vec4(1, 0, 0, 1), &global_knee);
	Gizmos::addAABBFilled(ankle_pos, vec3(0.5f), vec4(0, 1, 1, 1), &global_ankle);

	Gizmos::addLine(hip_pos, knee_pos, vec4(0, 1, 0, 1), vec4(1, 0, 0, 1));
	Gizmos::addLine(knee_pos, ankle_pos, vec4(0, 1, 0, 1), vec4(1, 0, 0, 1));

	///////////////////////////////

	// Moving Cube //

	//vec3 final_pos = glm::mix(m_positions[0],
	//	m_positions[1],
	//	sin_wave);
	//
	//glm::quat final_rot = glm::slerp(m_rotations[0],
	//									m_rotations[1],
	//										sin_wave);
	//
	////glm::toMat4  -  convert quaternion to a matrix
	//glm::mat4 transform = glm::translate(final_pos) * glm::toMat4(final_rot);
	//
	//Gizmos::addTransform(transform);
	//Gizmos::addAABBFilled(final_pos, vec3(1), vec4(0, 0, 1, 1), &transform);

	/////////////////


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

bool Quaternion::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}