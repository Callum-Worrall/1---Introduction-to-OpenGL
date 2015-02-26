#include "AdvancedTexturing.h"
#include "Utility.h"


#include "glm_include.h"
#include "Gizmos.h"
#include "stb_image.h"


bool AdvancedTexturing::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	m_draw_gizmos = true;


	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	//Generate Quad
	GenerateQuad(5.0f);

	//Load Texture Function
	LoadTextures("./textures/rock_diffuse.tga",
					"./textures/rock_normal.tga",
						"./textures/rock_specular.tga");


	Utility::LoadShader("./shaders/normal_mapped_vertex.glsl",
							"./shaders/normal_mapped_fragment.glsl",
								&m_program_ID);

	m_ambient_light = vec3(0.1f);
	m_light_dir = glm::normalize(vec3(-1, -1, 0));
	m_light_color = vec3(0.7f);
	m_material_color = vec3(1);
	m_specular_power = 15.0f;

	///////////

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);

	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(8);
	m_camera->SetSensitivity(10);

	m_background_color = vec4(0.5, 0.5, 0.5, 1);

	m_lightingControl = new GUI();
	m_lightingControl->StartUp("Lighting Bar", 1280, 720, m_window);
	TwAddSeparator(m_lightingControl->GetBar(), "LIGHT BAR", "");
	TwAddVarRW(m_lightingControl->GetBar(), "Background Colour", TW_TYPE_COLOR4F, &m_background_color, "");
	TwAddVarRW(m_lightingControl->GetBar(), "Light Direction", TW_TYPE_DIR3F, &m_light_dir, "group = Light");
	TwAddVarRW(m_lightingControl->GetBar(), "Light Colour", TW_TYPE_COLOR3F, &m_light_color, "group = Light");
	TwAddVarRW(m_lightingControl->GetBar(), "Specular Power", TW_TYPE_FLOAT, &m_specular_power, "group = Light min = 0 max = 100 step = 0.05");
	TwAddVarRW(m_lightingControl->GetBar(), "Draw Gizmos", TW_TYPE_BOOL8, &m_draw_gizmos, "");
	TwAddVarRO(m_lightingControl->GetBar(), "FPS", TW_TYPE_FLOAT, &m_fps, "precision = 5");


	return true;
}

bool AdvancedTexturing::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	TwDeleteAllBars();
	TwTerminate();

	Application::ShutDown();

	return true;
}

bool AdvancedTexturing::Update()
{
	if (Application::Update() == false)
	{
		return false;
	}

	glClearColor(m_background_color.x,
					m_background_color.y,
					m_background_color.z,
					m_background_color.w);

	float DeltaTime = (float)glfwGetTime();

	//Set time to 0
	glfwSetTime(0.0f);


	m_fps = 1 / DeltaTime;


	//

	m_light_dir = (glm::rotate(DeltaTime,
					vec3(0, 1, 0)) * vec4(m_light_dir, 0)).xyz;
	
	//

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

bool AdvancedTexturing::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program_ID);

	int proj_view_uniform =
		glGetUniformLocation(m_program_ID, "projection_view");

	glUniformMatrix4fv(proj_view_uniform, 1,
		GL_FALSE, (float*)&m_camera->GetProjectionView());

	int ambient_uniform =
		glGetUniformLocation(m_program_ID, "ambient_light");
	int light_dir_uniform =
		glGetUniformLocation(m_program_ID, "light_dir");
	int light_color_uniform =
		glGetUniformLocation(m_program_ID, "light_color");
	int eye_pos_uniform =
		glGetUniformLocation(m_program_ID, "eye_pos");
	int specular_power_uniform =
		glGetUniformLocation(m_program_ID, "specular_power");

	glUniform3fv(ambient_uniform, 1, (float*)&m_ambient_light);
	glUniform3fv(light_dir_uniform, 1, (float*)&m_light_dir);
	glUniform3fv(light_color_uniform, 1, (float*)&m_light_color);


	vec3 camera_pos = m_camera->GetWorldTransform()[3].xyz;
	glUniform3fv(eye_pos_uniform, 1, (float*)&camera_pos);

	glUniform1f(specular_power_uniform, m_specular_power);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	int diffuse_location =
		glGetUniformLocation(m_program_ID, "diffuse_tex");

	int normal_location =
		glGetUniformLocation(m_program_ID, "normal_tex");

	int specular_location =
		glGetUniformLocation(m_program_ID, "specular_tex");

	glUniform1i(diffuse_location, 0);
	glUniform1i(normal_location, 1);
	glUniform1i(specular_location, 2);

	glBindVertexArray(m_quad.m_VAO);
	glDrawElements(GL_TRIANGLES, m_quad.m_index_count, GL_UNSIGNED_INT, 0);

	if (m_draw_gizmos == true)
	{
		Gizmos::draw(m_camera->GetProjectionView());
	}
	
	//GUI
	m_lightingControl->Draw();
	
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void AdvancedTexturing::LoadTextures(const char* a_diffuse_file,
										const char* a_normal_file,
											const char* a_specular_file)
{
	int width = 0;
	int height = 0;
	int channels;

	//DIFFUSE
	unsigned char* data = stbi_load(a_diffuse_file,
										&width, &height,
											&channels, STBI_default);

	glGenTextures(1, &m_diffuse_texture);
	glBindTexture(GL_TEXTURE_2D, m_diffuse_texture);

	//,type of channel, actual data providing it (image)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
								GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);
	
	////////


	//NORMAL
	data = stbi_load(a_normal_file,
							&width, &height,
								&channels, STBI_default);

	glGenTextures(1, &m_normal_texture);
	glBindTexture(GL_TEXTURE_2D, m_normal_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
								GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	////////


	//SPECULAR
	data = stbi_load(a_specular_file,
							&width, &height,
								&channels, STBI_default);

	glGenTextures(1, &m_specular_texture);
	glBindTexture(GL_TEXTURE_2D, m_specular_texture);

	//,type of channel, actual data providing it (image)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
								GL_RGB, GL_UNSIGNED_BYTE, data);

	//Texture Filtering Options			(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR or GL_NEAREST_MIPMAP_NEAREST)
	//What to use when pixels are too big 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//What to use when pixels are too small
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	////////
}

void AdvancedTexturing::GenerateQuad(float size)
{
	VertexNormal vertex_data[4];

	vertex_data[0].position = vec4(-size, 0, -size, 1);
	vertex_data[1].position = vec4(-size, 0, size, 1);
	vertex_data[2].position = vec4(size, 0, size, 1);
	vertex_data[3].position = vec4(size, 0, -size, 1);

	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);
	vertex_data[0].normal = vec4(0, 1, 0, 0);

	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);
	vertex_data[0].tangent = vec4(1, 0, 0, 0);

	vertex_data[0].tex_coord = vec2(0, 0);
	vertex_data[1].tex_coord = vec2(0, 1);
	vertex_data[2].tex_coord = vec2(1, 1);
	vertex_data[3].tex_coord = vec2(1, 0);

	unsigned int index_data[6] = { 0, 2, 1, 0, 3, 2 };
	m_quad.m_index_count = 6;

	glGenVertexArrays(1, &m_quad.m_VAO);
	glGenBuffers(1, &m_quad.m_VBO);
	glGenBuffers(1, &m_quad.m_IBO);

	glBindVertexArray(m_quad.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_quad.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNormal)* 4, vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* 6, index_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//normal
	glEnableVertexAttribArray(2);	//tangent
	glEnableVertexAttribArray(3);	//tex coord

	//Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(VertexNormal), 0); 

	//Normal
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
		sizeof(VertexNormal), (void*)(sizeof(vec4) * 1)); 

	//Tangent
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,
		sizeof(VertexNormal), (void*)(sizeof(vec4) * 2)); 

	//Tex Coord
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexNormal), (void*)(sizeof(vec4) * 3)); 

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}