#include "AppTemplate.h"
#include "Utility.h"


bool AppTemplate::StartUp() // START UP //
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);



	//Insert Custom Code Here//

	//Load Shaders
	Utility::LoadShader("./shaders/lighting_vertex.glsl",
						"./shaders/lighting_fragment.glsl",
							&m_program_ID);

	//Load Shapes
	std::vector<tinyobj::shape_t> shapes;
	//Load Materials
	std::vector<tinyobj::material_t> materials;

	//Load OBJ File
	std::string err = tinyobj::LoadObj(shapes, materials, "./objs/dragon.obj");

	//Check OBJ Loaded Correctly
	if (err.size() != 0)
	{
		return false;
	}

	//Create Buffers
	CreateOpenGLBuffers(shapes);

	//Lighting Shader Variables
	m_ambient_light = vec3(0.1f);
	m_light_dir = vec3(0, -1, 0);
	m_light_color = vec3(0.6f, 0, 0);
	m_material_color = vec3(1);
	m_f_specular_power = 15.0f;

	///////////////////////////

	Gizmos::create();

	m_camera = FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);

	m_camera.SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera.SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera.SetSpeed(3);

	return true;
}


bool AppTemplate::ShutDown()	// SHUT DOWN //
{
	Gizmos::destroy();

	Application::ShutDown();

	return true;
}


bool AppTemplate::Update()	// UPDATE //
{
	if (Application::Update() == false)
	{
		return false;
	}

	float DeltaTime = (float)glfwGetTime();

	//Set time to 0
	glfwSetTime(0.0f);

	//Insert Custom Code Here//
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShader();
	}

	//Camera Update
	m_camera.Update(DeltaTime);

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	// GRID //  
	//Draw Grid 20 x 20
	for (int i = 0; i <= 20; i++)
	{
		//x == -10 + i
		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
			i == 10 ? white : black);

		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//////////

	return true;
}

bool AppTemplate::Draw()	// UPDATE //
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Insert Custom Code Here//
	

	glUseProgram(m_program_ID);

	int proj_view_uniform = glGetUniformLocation(m_program_ID, "projection_view");
	glUniformMatrix4fv(proj_view_uniform, 1, GL_FALSE, (float*)&m_camera.GetProjectionView());

	int ambient_uniform = glGetUniformLocation(m_program_ID, "ambient_light");
	int light_dir_uniform = glGetUniformLocation(m_program_ID, "light_dir");
	int light_color_uniform = glGetUniformLocation(m_program_ID, "light_color");
	int material_uniform = glGetUniformLocation(m_program_ID, "material_color");

	int eye_pos_uniform =
		glGetUniformLocation(m_program_ID, "eye_pos");
	int specular_power_uniform =
		glGetUniformLocation(m_program_ID, "specular_power");

	glUniform3fv(ambient_uniform, 1, (float*)&m_ambient_light);
	glUniform3fv(light_dir_uniform, 1, (float*)&m_light_dir);
	glUniform3fv(light_color_uniform, 1, (float*)&m_light_color);
	glUniform3fv(material_uniform, 1, (float*)&m_material_color);

	vec3 camera_pos = m_camera.GetWorldTransform()[3].xyz;
	glUniform3fv(eye_pos_uniform, 1, (float*)&camera_pos);

	glUniform1f(specular_power_uniform, m_f_specular_power);

	for (unsigned int mesh_index = 0; mesh_index < m_gl_data.size(); mesh_index++)
	{
		glBindVertexArray(m_gl_data[mesh_index].m_VAO);

		glDrawElements(GL_TRIANGLES, m_gl_data[mesh_index].m_index_count, GL_UNSIGNED_INT, 0);
	}

	///////////////////////////


	Gizmos::draw(m_camera.GetProjectionView());

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void AppTemplate::CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes)
{
	//?
	m_gl_data.resize(shapes.size());

	//?
	for (unsigned int mesh_index = 0; mesh_index < shapes.size(); ++mesh_index)
	{
		//?
		unsigned int float_count = shapes[mesh_index].mesh.positions.size();
		float_count += shapes[mesh_index].mesh.normals.size();
		float_count += shapes[mesh_index].mesh.texcoords.size();

		//Create Vertex Data Vector
		std::vector<float> vertex_data;

		//?
		vertex_data.reserve(float_count);

		//Inserting Shape Mesh Vertices Data (Inserting at the first Element as it's empty)
		vertex_data.insert(vertex_data.end(),
			//We want to insert from the start,
			shapes[mesh_index].mesh.positions.begin(),
			// to the end
			shapes[mesh_index].mesh.positions.end());

		//Inserting  GL Data 
		vertex_data.insert(vertex_data.end(),
			//We want to insert from the start,
			shapes[mesh_index].mesh.normals.begin(),
			// to the end
			shapes[mesh_index].mesh.normals.end());

		//Set the Data's Index Count to the size of the Mesh's Indices
		m_gl_data[mesh_index].m_index_count =
			shapes[mesh_index].mesh.indices.size();


		//Standard OpenGL Start Up Code//
		/* Including:
			Vertex Array Object
			Vertex Buffer Object
			Index Buffer Object
		*/

		//Generate Buffers (Array, Vertex and Index)
		glGenVertexArrays(1, &m_gl_data[mesh_index].m_VAO);
		glGenBuffers(1, &m_gl_data[mesh_index].m_VBO);
		glGenBuffers(1, &m_gl_data[mesh_index].m_IBO);

		//Bind Data to Array Buffer
		glBindVertexArray(m_gl_data[mesh_index].m_VAO);

		//Bind the Vertex Buffer to the Vertex Array
		glBindBuffer(GL_ARRAY_BUFFER, m_gl_data[mesh_index].m_VBO);

		//Insert Data into (OpenGL?)
		glBufferData(GL_ARRAY_BUFFER,
			vertex_data.size() * sizeof(float),
			vertex_data.data(), GL_STATIC_DRAW);

		//Bind the Data to the Index (Array) Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_data[mesh_index].m_IBO);

		//?
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			shapes[mesh_index].mesh.indices.size() * sizeof(unsigned int),
			shapes[mesh_index].mesh.indices.data(), GL_STATIC_DRAW);

		//Enable the Two Attributes to Create the Format for the Buffer Arrays
		glEnableVertexAttribArray(0); //Position Attrib.
		glEnableVertexAttribArray(1); //Normal Data Attrib.

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0,
			(void*)(sizeof(float)*shapes[mesh_index].mesh.positions.size()));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		/////////////////////////////////
	}
}

void AppTemplate::CleanUpOpenGLBuffers()
{
	glDeleteProgram(m_program_ID);

	for (unsigned int i = 0; i < m_gl_data.size(); i++)
	{
		glDeleteVertexArrays(1, &m_gl_data[i].m_VAO);
		glDeleteBuffers(1, &m_gl_data[i].m_VBO);
		glDeleteBuffers(1, &m_gl_data[i].m_IBO);
	}
}

void AppTemplate::ReloadShader()
{
	glDeleteProgram(m_program_ID);

	Utility::LoadShader("./shaders/lighting_vertex.glsl",
						"./shaders/lighting_fragment.glsl",
												&m_program_ID);
}