#include "PerlinNoise.h"
#include "Utility.h"


bool PerlinNoise::StartUp()
{
	if (Application::StartUp() == false)
	{
		return false;
	}

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	m_camera = new FlyCamera(1280.0f, 720.0f, 10.0f, 5.0f);
	m_camera->SetPerspective(glm::radians(60.0f), 16 / 9.f, 0.1f, 1000.f);
	m_camera->SetLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera->SetSpeed(15);

	gPressed = false;

	gridActive = true;

	/////////////////////

	//Generate Plane
	BuildGrid(vec2(30, 30), glm::ivec2(128, 128));

	//Load Texture


	m_dims = glm::ivec2(128, 128);
	m_octaves = 6;
	m_persistence = 0.6f;
	m_scale = 12.0f;

	lastDims = m_dims;
	lastOctaves = m_octaves;
	lastPersistence = m_persistence;

	BuildPerlinTexture(m_dims, m_octaves, m_persistence);



	//Load Shaders
	LoadShaders("./shaders/perlin_vertex.glsl", nullptr, "./shaders/perlin_fragment.glsl", &m_program_id);

	/////////////////////

	TerrainWindow.StartUp("Terrain", 1280, 720);

	TwAddSeparator(TerrainWindow.GetBar(), "TERRAIN BAR", "");

	//TerrainWindow.AddBarF("Dimension X", &(m_dims->x), "");
	//TerrainWindow.AddBarF("Dimension Y", &(m_dims->y), "");

	TerrainWindow.AddBarI("Octaves", &m_octaves, "min = 0 max = 24 step = 1");
	TerrainWindow.AddBarF("Persisistence", &m_persistence, "min = 0.0f max = 0.99f step = 0.03");
	TerrainWindow.AddBarF("Scale", &m_scale, "");

	/////////////////////

	srand(time(0));

	return true;
}


bool PerlinNoise::ShutDown()
{
	delete m_camera;

	Gizmos::destroy();

	Application::ShutDown();

	return true;
}

bool PerlinNoise::Update()
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


	if (m_dims != lastDims || lastOctaves != m_octaves || lastPersistence != m_persistence)
	{
		BuildPerlinTexture(m_dims, m_octaves, m_persistence);
		lastDims = m_dims;
		lastOctaves = m_octaves;
		lastPersistence = m_persistence;
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

bool PerlinNoise::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Gizmos::draw(m_camera->GetProjectionView());

	/////////////////////


	glUseProgram(m_program_id);

	int view_proj_uniform =
		glGetUniformLocation(m_program_id, "view_proj");
	glUniformMatrix4fv(view_proj_uniform, 1,
		GL_FALSE, (float*)&m_camera->GetProjectionView());

	int perlin_texture_uniform =
		glGetUniformLocation(m_program_id, "perlin_texture");
	glUniform1i(perlin_texture_uniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);


	int scale_uniform =
		glGetUniformLocation(m_program_id, "scale");
	glUniform1f(scale_uniform, m_scale);

	int persistence_uniform =
		glGetUniformLocation(m_program_id, "persistence");
	glUniform1f(persistence_uniform, m_persistence);

	int min_uniform =
		glGetUniformLocation(m_program_id, "min");
	glUniform1f(min_uniform, MIN);

	int max_uniform =
		glGetUniformLocation(m_program_id, "max");
	glUniform1f(max_uniform, MAX);

	glBindVertexArray(m_plane_mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, m_plane_mesh.m_index_count, GL_UNSIGNED_INT, 0);


	/////////////////////


	TerrainWindow.Draw();


	////////////////////

	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return true;
}

void PerlinNoise::Input()
{
	GLFWwindow* window = glfwGetCurrentContext();

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && gPressed == false)
	{
		gPressed = true;
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

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		ReloadShader();
	}
}

void PerlinNoise::BuildGrid(vec2 real_dims, glm::ivec2 dims)
{
	//compute how many vertices we need
	unsigned int vertex_count = (dims.x + 1) * (dims.y + 1);

	//allocate vertex data
	VertexTexCoord* vertex_data = new VertexTexCoord[vertex_count];

	//compute how many indices we need
	unsigned int index_count = dims.x * dims.y * 6;

	//allocate index data
	unsigned int* index_data = new unsigned int[index_count];

	//create two nested FOR loops
	float curr_y = -real_dims.y / 2.0f;
	for (int y = 0; y < dims.y + 1; ++y)
	{
		float curr_x = -real_dims.x / 2.0f;
		for (int x = 0; x < dims.x + 1; ++x)
		{
			//create points
			vertex_data[y * (dims.x + 1) + x].position =
				vec4(curr_x, 0, curr_y, 1);

			vertex_data[y * (dims.x + 1) + x].tex_coord =
				vec2((float)x / (float)dims.x, (float)y / (float)dims.y);

			curr_x += real_dims.x / (float)dims.x;
		}
		curr_y += real_dims.y / (float)dims.y;
	}

	//generate our index_data
	int curr_index = 0;
	for (int y = 0; y < dims.y; ++y)
	{
		for (int x = 0; x < dims.x; x++)
		{
			//create 6 indices for each quad

			//traingle 1
			index_data[curr_index++] = y * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + x;
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);

			//triangle 2
			index_data[curr_index++] = (y + 1) * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + (x + 1);
			index_data[curr_index++] = y * (dims.x + 1) + x;
		}
	}

	m_plane_mesh.m_index_count = index_count;

	//create VertexArrayObject, buffers etc
	glGenVertexArrays(1, &m_plane_mesh.m_VAO);
	glGenBuffers(1, &m_plane_mesh.m_VBO);
	glGenBuffers(1, &m_plane_mesh.m_IBO);

	//bind and fill buffers
	glBindVertexArray(m_plane_mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_plane_mesh.m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_plane_mesh.m_IBO);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(VertexTexCoord) * vertex_count, vertex_data, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * index_count, index_data, GL_STATIC_DRAW);



	//tell opengl about our vertex structure
	glEnableVertexAttribArray(0);	//position
	glEnableVertexAttribArray(1);	//tex coord

	//Position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
							sizeof(VertexTexCoord), 0);

	//Tex Coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
							sizeof(VertexTexCoord), (void*)sizeof(vec4));

	//unbind stuff
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//free data
	delete[] vertex_data;
	delete[] index_data;
}

void PerlinNoise::BuildPerlinTexture(glm::ivec2 dims, int octaves, float persistence)
{
	//set scale
	float scale = (1.0f / dims.x) * 3.0f;

	//allocate memory for perlin data
	m_perlin_data = new float[dims.x * dims.y];

	MIN = FLT_MAX;
	MAX = -FLT_MAX;

	//loop through all the pixels
	for (int y = 0; y < dims.y; y++)
	{
		for (int x = 0; x < dims.x; x++)
		{
			float amplitude = 1;
			float frequency = 1;

			//generate the perlin noise
			m_perlin_data[y * dims.x + x] = 0;

			//loop over  anumber of octaves
			for (int o = 0; o < octaves; o++)
			{
				//each octave will double the frequency of the previous one
				//we scale the points to achieve this
				float perlin_sample =
					glm::perlin(vec2(x, y) * scale * frequency) * 0.5f + 0.5f; //call glm::perlin function

				//each subsequent octave will contribute less and less to the final height
				//to achieve this, mutliply by a smaller and smaller number

				perlin_sample *= amplitude;
				m_perlin_data[y * dims.x + x] += perlin_sample;

				if (m_perlin_data[y * dims.x + x] < MIN)
				{
					MIN = m_perlin_data[y * dims.x + x];
				}
				if (m_perlin_data[y * dims.x + x] > MAX)
				{
					MAX = m_perlin_data[y * dims.x + x];
				}
				
				amplitude *= persistence;
				frequency *= 2.0f;
			}
		}
	}

	//generate opengl texture handles
	glGenTextures(1, &m_perlin_texture);
	glBindTexture(GL_TEXTURE_2D, m_perlin_texture);


	//pass perlin data to texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, dims.x, dims.y, 0,
								GL_RED, GL_FLOAT, m_perlin_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void PerlinNoise::ReloadShader()
{
	glDeleteProgram(m_program_id);

	LoadShaders("./shaders/perlin_vertex.glsl", nullptr, "./shaders/perlin_fragment.glsl", &m_program_id);
}