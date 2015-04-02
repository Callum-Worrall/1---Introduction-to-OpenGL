#include "AdvancedNavigation.h"
#include "Utility.h"

bool AdvancedNavigation::StartUp()
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

	gPressed = false;

	/////////////////////



	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, "./objs/sponza/SponzaSimple.obj");
	LoadShaders("./shaders/navmesh_vertex.glsl", nullptr, "./shaders/navmesh_fragment.glsl", &m_program_ID);
	BuildNavigationMesh();


	/////////////////////

	return true;
}


bool AdvancedNavigation::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool AdvancedNavigation::Update()
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

	Gizmos::clear();

	Input();

	/////////////////////

	vec3 raise = vec3(0, 0.1f, 0);
	for (unsigned int node_index = 0; node_index < m_nav_nodes.size(); node_index++)
	{
		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[0] + raise, vec3(0.1f), vec4(1, 0, 0, 1));
		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[1] + raise, vec3(0.1f), vec4(1, 0, 0, 1));
		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[3] + raise, vec3(0.1f), vec4(1, 0, 0, 1));

		Gizmos::addAABBFilled(m_nav_nodes[node_index].position + raise, vec3(0.1f), vec4(0, 0, 1, 1));

		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[0] + raise, m_nav_nodes[node_index].corners[1] + raise, vec4(0, 1, 0, 1));
		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[1] + raise, m_nav_nodes[node_index].corners[2] + raise, vec4(0, 1, 0, 1));
		Gizmos::addAABBFilled(m_nav_nodes[node_index].corners[3] + raise, m_nav_nodes[node_index].corners[0] + raise, vec4(0, 1, 0, 1));

		for (unsigned int edge = 0; edge < 3; edge++)
		{
			Gizmos::addLine(m_nav_nodes[node_index].position + raise, m_nav_nodes[node_index].edges[edge]->position + raise, vec4(1, 1, 0, 1));
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

bool AdvancedNavigation::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////


	//start using the program
	glUseProgram(m_program_ID);

	//pass in the projection view (the camera)
	int proj_view_uniform =
		glGetUniformLocation(m_program_ID, "projection_view");

	glUniformMatrix4fv(proj_view_uniform, 1, GL_FALSE,
		(float*)&m_camera->GetProjectionView());

	glBindVertexArray(m_sponza.m_VAO);

	glDrawElements(GL_TRIANGLES, m_sponza.m_index_count, GL_UNSIGNED_INT, 0);


	/////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void AdvancedNavigation::Input()
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


void AdvancedNavigation::BuildNavigationMesh()
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, "./objs/sponza/SponzaSimpleNavMesh.obj");

	if (err.size() != 0)
	{
		printf("%s\n", err.c_str());
		return;
	}

	if (shapes.size() != 1)
	{
		printf("OBJ IS NOT LOADING\n");
		return;
	}

	tinyobj::mesh_t* mesh = &shapes[0].mesh;

	unsigned int index_count = mesh->indices.size();

	for (unsigned int index = 0; index < index_count; index += 3)
	{
		unsigned int node_index = index / 3;

		for (unsigned int corner_index = 0; corner_index < 3; ++corner_index)
		{
			unsigned int vertex_index = mesh->indices[index + corner_index];
			m_nav_nodes[node_index].corners[corner_index].x = mesh->positions[vertex_index * 3 + 0];

		}
	}
}