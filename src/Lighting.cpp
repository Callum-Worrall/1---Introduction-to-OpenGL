//#include "Lighting.h"
//#include "FBXFile.h"
//#include "Utility.h"
//
//Lighting::Lighting()
//{
//
//}
//
//
//Lighting::~Lighting()
//{
//
//}
//
////Inverse
//
//bool Lighting::StartUp()
//{
//	if (Application::StartUp() == false)
//	{
//		return false;
//	}
//
//	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//
//	//Load FBX File
//	m_file = new FBXFile();
//	m_file->load("./data/models/stanford/Bunny.fbx");
//
//	CreateOpenGLBuffers(m_file);
//
//	Utility::LoadShader("./shaders/lighting_vertex.glsl", "./shaders/lighting_fragment.glsl", &m_program_ID);
//
//	Gizmos::create();
//
//	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f);
//
//	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
//	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
//	m_camera->SetSpeed(3);
//
//	return true;
//}
//
//
//bool Lighting::ShutDown()
//{
//	delete m_camera;
//
//	Gizmos::destroy();
//
//	Application::ShutDown();
//
//	return true;
//}
//
//bool Lighting::Update()
//{
//	if (Application::Update() == false)
//	{
//		return false;
//	}
//
//	float DeltaTime = (float)glfwGetTime();
//
//	//Set time to 0
//	glfwSetTime(0.0f);
//
//	//Camera Update
//	m_camera->Update(DeltaTime);
//
//	vec4 white(1);
//	vec4 black(0, 0, 0, 1);
//
//	//Draw Grid 20 x 20
//	for (int i = 0; i <= 20; i++)
//	{
//		//x == -10 + i
//		Gizmos::addLine(vec3(-10 + i, 0, -10), vec3(-10 + i, 0, 10),
//			i == 10 ? white : black);
//
//		Gizmos::addLine(vec3(-10, 0, -10 + i), vec3(10, 0, -10 + i),
//			i == 10 ? white : black);
//	}
//
//	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)
//	{
//
//	}
//
//	return true;
//}
//
//bool Lighting::Draw()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glUseProgram(m_program_ID);
//
//	// bind the camera
//	int proj_view_uniform = glGetUniformLocation(m_program_ID, "projection_view");
//	glUniformMatrix4fv(proj_view_uniform, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));
//
//	// bind our vertex array object and draw the mesh
//	for (unsigned int mesh_index = 0; mesh_index < m_file->getMeshCount(); ++mesh_index)
//	{
//		////FBX Mesh
//		//FBXMeshNode* mesh = m_file->getMeshByIndex(mesh_index);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glBindVertexArray(glData[0]);
//
//		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
//	}
//
//
//	Gizmos::draw(m_camera->GetProjectionView());
//
//	glfwSwapBuffers(m_window);
//	glfwPollEvents();
//
//	return true;
//}
//
//
//void Lighting::CreateOpenGLBuffers(FBXFile* a_file)
//{
//	for (unsigned int mesh_index = 0; mesh_index < a_file->getMeshCount(); mesh_index++)
//	{
//		//FBXMeshNode* mesh = a_file->getMeshByIndex(mesh_index);
//
//		//contains [0] is VAO, [1] is VBO and [3] is IBO
//		unsigned int* gl_data = new unsigned int[3];
//
//		glGenVertexArrays(1, &gl_data[0]);
//		glGenBuffers(2, &gl_data[1]);
//
//		glBindVertexArray(gl_data[0]);
//
//		glBindBuffer(GL_ARRAY_BUFFER, gl_data[1]);
//		glBufferData(GL_ARRAY_BUFFER,
//			//mesh->m_vertices.size() * sizeof(FBXVertex),
//			//mesh->m_vertices.data(), GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_data[2]);
//		glBufferData(GL_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int),
//			mesh->m_indices.data(), GL_STATIC_DRAW);
//
//		glEnableVertexAttribArray(0);	//positon
//		glEnableVertexAttribArray(1);	//normal data
//
//		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
//		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (void*)(FBXVertex::NormalOffset));
//
//
//		glBindVertexArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//		mesh->m_userData = gl_data;
//	}
//}
//
//void Lighting::CleanUpOpenGLBuffers(FBXFile* a_file)
//{
//	// clean up the vertex data attached to each mesh
//	for (unsigned int mesh_index = 0; mesh_index < a_file->getMeshCount(); ++mesh_index)
//	{
//		FBXMeshNode* mesh = a_file->getMeshByIndex(mesh_index);
//
//		unsigned int* glData = (unsigned int*)mesh->m_userData;
//
//		glDeleteVertexArrays(1, &glData[0]);
//
//		glDeleteBuffers(1, &glData[1]);
//		glDeleteBuffers(1, &glData[2]);
//
//		delete[] glData;
//	}
//}
//
//
//void Lighting::ReloadShader()
//{
//	glDeleteProgram(m_program_ID);
//
//	Utility::LoadShader("./shaders/lighting_vertex.glsl", "./shaders/lighting_fragment.glsl", &m_program_ID);
//}