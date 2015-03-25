#include "SceneManagement.h"


#include "tiny_obj_loader.h"


//AABB GenerateAABB(vec3* positions, unsigned int count, unsigned int stride)
//{
//
//
//	return result;
//}
//
//void RenderAABB(AABB aabb)
//{
//
//}

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


	////Load Meshes
	//LoadMesh("./objs/dragon.obj");


	//m_ambient_light = vec3(0.1f);
	//m_light_dir = vec3(0, -1, 0);
	//m_light_color = vec3(0.6f, 0, 0);
	//m_material_color = vec3(1);
	//m_specular_power = 15.0f;


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
	//	printf("On the plane\n");


	////Plane Check
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
	planes[0] = vec4(
		transform[0][3] - transform[1][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);

	// left side
	planes[1] = vec4(
		transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);

	// top
	planes[2] = vec4(
		transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);

	// bottom
	planes[3] = vec4(
		transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);

	// far
	planes[4] = vec4(
		transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);

	// near
	planes[5] = vec4(
		transform[0][3] + transform[0][2],
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


	//loop through meshes in scene

	//if in frustum, render meshes

	//loop through frustum planes

		//list meshes being rendered

	//else, not in frustum, do not render
		//list meshes that stopped being rendered
		


	//for (unsigned int mesh_index = 0; mesh_index < length; mesh_index++)
	//{
	//
	//}












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


//bool OnPositivePlaneSide(vec4 plane, AABB aabb)
//{
//	vec3 planeTestA, planeTestB;
//
//	if (plane.x >= 0)
//	{
//		planeTestA.x = aabb.min.x;
//		planeTestB.x = aabb.max.x;
//	}
//	else
//	{
//		planeTestA.x = aabb.max.x;
//		planeTestB.x = aabb.min.x;
//	}
//
//	if (plane.y >= 0)
//	{
//		planeTestA.y = aabb.min.y;
//		planeTestB.y = aabb.max.y;
//	}
//	else
//	{
//		planeTestA.y = aabb.max.y;
//		planeTestB.y = aabb.min.y;
//	}
//
//	if (plane.z >= 0)
//	{
//		planeTestA.z = aabb.min.z;
//		planeTestB.z = aabb.max.z;
//	}
//	else
//	{
//		planeTestA.z = aabb.max.z;
//		planeTestB.z = aabb.min.z;
//	}
//
//	float dA = dot(vec3(plane), planeTestA) + plane.w;
//	float dB = dot(vec3(plane), planeTestB) + plane.w;
//
//	return (dA >= 0, dB >= 0);
//}


//MeshObject SceneManagement::LoadMesh(char* obj_filename)
//{
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//
//	tinyobj::LoadObj(shapes, materials, obj_filename);
//
//	for (unsigned int shape_index = 0; shape_index < shapes.size(); shape_index++)
//	{
//
//		unsigned int meshCount = m_meshes.size();
//
//		m_meshes.resize(m_meshes.size() + shape_index);
//
//		std::vector<float> vertex_data;
//
//
//
//		unsigned int float_count = shapes[shape_index].mesh.positions.size();
//		float_count += shapes[shape_index].mesh.normals.size();
//		//float_count += shapes[mesh_index].mesh.texcoords.size();
//
//		vertex_data.reserve(float_count);
//
//		//Inserting Shape Mesh Vertices Data (Inserting at the first Element as it's empty)
//		vertex_data.insert(vertex_data.end(),
//			//We want to insert from the start,
//			shapes[shape_index].mesh.positions.begin(),
//			// to the end
//			shapes[shape_index].mesh.positions.end());
//
//		//Inserting  GL Data 
//		vertex_data.insert(vertex_data.end(),
//			//We want to insert from the start,
//			shapes[shape_index].mesh.normals.begin(),
//			// to the end
//			shapes[shape_index].mesh.normals.end());
//
//		//Set the Data's Index Count to the size of the Mesh's Indices
//		m_meshes[shape_index].m_index_count =
//			shapes[shape_index].mesh.indices.size();
//
//
//		//Standard OpenGL Start Up Code//
//
//		//Generate Buffers (Array, Vertex and Index)
//		glGenVertexArrays(1, &m_meshes[mesh_index].m_VAO);
//		glGenBuffers(1, &m_meshes[mesh_index].m_VBO);
//		glGenBuffers(1, &m_meshes[mesh_index].m_IBO);
//
//		//Bind Data to Array Buffer
//		glBindVertexArray(m_meshes[mesh_index].m_VAO);
//
//		//Bind the Vertex Buffer to the Vertex Array
//		glBindBuffer(GL_ARRAY_BUFFER, m_meshes[mesh_index].m_VBO);
//
//		//Insert Data into (OpenGL?)
//		glBufferData(GL_ARRAY_BUFFER,
//			vertex_data.size() * sizeof(float),
//			vertex_data.data(), GL_STATIC_DRAW);
//
//		//Bind the Data to the Index (Array) Buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshes[mesh_index].m_IBO);
//
//		//
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//			shapes[shape_index].mesh.indices.size() * sizeof(unsigned int),
//			shapes[shape_index].mesh.indices.data(), GL_STATIC_DRAW);
//
//		//Enable the Two Attributes to Create the Format for the Buffer Arrays
//		glEnableVertexAttribArray(0); //Position Attrib.
//		glEnableVertexAttribArray(1); //Normal Data Attrib.
//
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
//			(void*)(sizeof(float)*shapes[shape_index].mesh.positions.size()));
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//		m_meshes[mesh_index].m_aabb = GenerateAABB(
//			(vec3*)&shapes[shape_index]).mesh.positions[0],
//			shapes[shape_index].mesh.positions.size() / 3);
//	}
//}
//
//void SceneManagement::DrawMesh(MeshObject mesh)
//{
//	glBindVertexArray(mesh.m_VAO);
//
//	glDrawElements(GL_TRIANGLES, mesh.m_index_count, GL_UNSIGNED_INT, 0);
//}


