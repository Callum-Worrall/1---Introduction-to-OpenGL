#include "Emitter.h"
#include "Utility.h"

//Emitter::Emitter() :
//m_particles(nullptr), m_maxParticles(0),
//m_vertexData(nullptr), m_indexData(nullptr),
//m_firstDead(0)
//{
//
//}
//
//Emitter::~Emitter()
//{
//	delete[] m_particles;
//	delete[] m_vertexData;
//	delete[] m_indexData;
//
//	glDeleteVertexArrays(1, &m_buffers.m_VAO);
//	glDeleteBuffers(1, &m_buffers.m_VBO);
//	glDeleteBuffers(1, &m_buffers.m_IBO);
//}
//
//void Emitter::Initialize(
//	unsigned int a_maxParticles, vec3 a_position,
//	float a_emitRate,
//	float a_lifespanMin, float a_lifespanMax,
//	float a_velocityMin, float a_velocityMax,
//	float a_startSize, float a_endSize,
//	vec4 a_startcolor, vec4 a_endcolor)
//{
//	// set up emit timers
//	m_emitTimer = 0;
//	m_emitRate = 1.0f / a_emitRate;
//	// store all variables passed in
//	m_startColor = a_startcolor;
//	m_endColor = a_endcolor;
//	m_startSize = a_startSize;
//	m_endSize = a_endSize;
//	m_velocityMin = a_velocityMin;
//	m_velocityMax = a_velocityMax;
//	m_lifespanMin = a_lifespanMin;
//	m_lifespanMax = a_lifespanMax;
//	m_maxParticles = a_maxParticles;
//
//	// create particle array
//	m_particles = new Particle[m_maxParticles];
//	m_firstDead = 0;
//
//	// create the array of vertices for the particles
//	// 4 vertices per particle for a quad.
//	// will be filled during update
//	m_vertexData = new ParticleVertex[m_maxParticles * 4];//
//	// create the index buffeer data for the particles
//	// 6 indices per quad of 2 triangles
//	// fill it now as it never changes
//	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
//	for (unsigned int i = 0; i < m_maxParticles; ++i) {
//		indexData[i * 6 + 0] = i * 4 + 0;
//		indexData[i * 6 + 1] = i * 4 + 1;
//		indexData[i * 6 + 2] = i * 4 + 2;
//		indexData[i * 6 + 3] = i * 4 + 0;
//		indexData[i * 6 + 4] = i * 4 + 2;
//		indexData[i * 6 + 5] = i * 4 + 3;
//	}
//	// create opengl buffers
//	Utility::LoadShader("./shaders/particle_vertex.glsl", "./shaders/particle_fragment.glsl", &m_program_ID);
//	
//	delete[] indexData;
//}
//
//
//void Emitter::EmitParticles()
//{
//	// only emit if there is a dead particle to use
//	if (m_firstDead >= m_maxParticles)
//		return;
//	// resurrect the first dead particle
//	Particle& particle = m_particles[m_firstDead++];
//	// assign its starting position
//	particle.position = m_position;
//	// randomise its lifespan
//	particle.lifetime = 0;
//	particle.lifespan = (rand() / (float)RAND_MAX) *
//		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;
//	// set starting size and color
//	particle.color = m_startColor;
//	particle.size = m_startSize;
//	// randomise velocity direction and strength
//	float velocity = (rand() / (float)RAND_MAX) *
//		(m_velocityMax - m_velocityMin) + m_velocityMin;
//	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
//	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
//	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
//	particle.velocity = glm::normalize(particle.velocity) *
//		velocity;
//}
//
//void Emitter::Update(float a_dt, const glm::mat4& a_cameraTransform)
//{
//	using glm::vec3;
//	using glm::vec4;
//	// spawn particles
//	m_emitTimer += a_dt;
//	while (m_emitTimer > m_emitRate)
//	{
//		EmitParticles();
//		m_emitTimer -= m_emitRate;
//	}
//
//	unsigned int quad = 0;
//	// update particles and turn live particles into billboard quads
//	for (unsigned int i = 0; i < m_firstDead; i++)
//	{
//		Particle* particle = &m_particles[i];
//
//		particle->lifetime += a_dt;
//
//		if (particle->lifetime >= particle->lifespan)
//		{
//			// swap last alive with this one
//			*particle = m_particles[m_firstDead - 1];
//			m_firstDead--;
//		}
//		else
//		{
//			// move particle
//			particle->position += particle->velocity * a_dt;
//			// size particle
//			particle->size = glm::mix(m_startSize, m_endSize,
//				particle->lifetime / particle->lifespan);
//			// color particle
//			particle->color = glm::mix(m_startColor, m_endColor,
//				particle->lifetime / particle->lifespan);
//			// make a quad the correct size and color
//			float halfSize = particle->size * 0.5f;
//			m_vertexData[quad * 4 + 0].position = vec4(halfSize,
//				halfSize, 0, 1);
//			m_vertexData[quad * 4 + 0].color = particle->color;
//			m_vertexData[quad * 4 + 1].position = vec4(-halfSize,
//				halfSize, 0, 1);
//			m_vertexData[quad * 4 + 1].color = particle->color;
//			m_vertexData[quad * 4 + 2].position = vec4(-halfSize,
//				-halfSize, 0, 1);
//			m_vertexData[quad * 4 + 2].color = particle->color;
//			m_vertexData[quad * 4 + 3].position = vec4(halfSize,
//				-halfSize, 0, 1);
//			m_vertexData[quad * 4 + 3].color = particle->color;
//			// create billboard transform
//			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3]) -
//				particle->position);
//			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
//			vec3 yAxis = glm::cross(zAxis, xAxis);
//			glm::mat4 billboard(vec4(xAxis, 0),
//				vec4(yAxis, 0),
//				vec4(zAxis, 0),
//				vec4(0, 0, 0, 1));
//
//			m_vertexData[quad * 4 + 0].position = billboard *
//				m_vertexData[quad * 4 + 0].position +
//				vec4(particle->position, 0);
//			m_vertexData[quad * 4 + 1].position = billboard *
//				m_vertexData[quad * 4 + 1].position +
//				vec4(particle->position, 0);
//			m_vertexData[quad * 4 + 2].position = billboard *
//				m_vertexData[quad * 4 + 2].position +
//				vec4(particle->position, 0);
//			m_vertexData[quad * 4 + 3].position = billboard *
//				m_vertexData[quad * 4 + 3].position +
//				vec4(particle->position, 0);
//			++quad;
//		}
//	}
//}
//
//
//void Emitter::Render()
//{
//	// sync the particle vertex buffer
//	// based on how many alive particles there are
//	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 *
//		sizeof(ParticleVertex), m_vertexData);
//	// draw particles
//	glBindVertexArray(m_buffers.m_VAO);
//	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
//}





Emitter::Emitter() :
m_particles(nullptr), m_maxParticles(0), m_aliveCount(0),
m_vertexData(nullptr), m_indexData(nullptr)
{
	m_buffers = {};
}

Emitter::~Emitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_buffers.m_VAO);
	glDeleteBuffers(1, &m_buffers.m_VBO);
	glDeleteBuffers(1, &m_buffers.m_IBO);
}

void Emitter::Initialize(
	unsigned int a_maxParticles, vec3 a_position,
	float a_emitRate,
	float a_lifespanMin, float a_lifespanMax,
	float a_velocityMin, float a_velocityMax,
	float a_startSize, float a_endSize,
	vec4 a_startcolor, vec4 a_endcolor)
{
	m_emitTimer = 0;
	m_emitRate = a_emitRate;
	
	//set passed in values
	m_position = vec4(a_position, 1);

	m_startColor = a_startcolor;
	m_endColor = a_endcolor;

	m_startSize = a_startSize;
	m_endSize = a_endSize;

	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;

	m_lifespanMin = a_lifespanMin;
	m_lifespanMax = a_lifespanMax;

	m_maxParticles = a_maxParticles;

	m_aliveCount = 0;

	// create the particle array
	m_particles = new Particle[m_maxParticles];

	// create the array of vertices for the particles
	// 4 Vertices per Particle for every Quad.
	// m_vertexData consistently filled during the Update()
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// create the index buffer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	m_indexData = new unsigned int[m_maxParticles * 6];

	for (unsigned int i = 0; i < m_maxParticles; ++i) {
		m_indexData[i * 6 + 0] = i * 4 + 0;
		m_indexData[i * 6 + 1] = i * 4 + 1;
		m_indexData[i * 6 + 2] = i * 4 + 2;
		m_indexData[i * 6 + 3] = i * 4 + 0;
		m_indexData[i * 6 + 4] = i * 4 + 2;
		m_indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers
	glGenVertexArrays(1, &m_buffers.m_VAO);
	glBindVertexArray(m_buffers.m_VAO);
	glGenBuffers(1, &m_buffers.m_VBO);
	glGenBuffers(1, &m_buffers.m_IBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
		sizeof(ParticleVertex), m_vertexData,
		GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers.m_IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
		sizeof(unsigned int), m_indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] m_indexData;
}


void Emitter::EmitParticles()
{
	unsigned int particles_to_emit =
		(unsigned int)(m_emitTimer * m_emitRate);

	m_emitTimer -= particles_to_emit / m_emitRate;


	for (unsigned int i = 0;
		i < particles_to_emit && m_aliveCount < m_maxParticles;
		i++)
	{
		// assign its starting position
		m_particles[m_aliveCount].position = m_position;

		// randomise its lifespan
		m_particles[m_aliveCount].lifetime = 0;
		m_particles[m_aliveCount].lifespan = glm::linearRand(m_lifespanMin, m_lifespanMax);

		// set starting size and color
		m_particles[m_aliveCount].color = m_startColor;
		m_particles[m_aliveCount].size = m_startSize;

		float velocityLength = glm::linearRand(m_velocityMin, m_velocityMax);

		m_particles[m_aliveCount].velocity.xyz = glm::sphericalRand(velocityLength);
		m_particles[m_aliveCount].velocity.w = 0;

		++m_aliveCount;
	}
}

void Emitter::Update(float a_dt)
{	
	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_aliveCount; ++i)
	{
		m_particles[i].lifetime += a_dt;

		if (m_particles[i].lifetime >= m_particles[i].lifespan)
		{
			// swap last alive with this one
			--m_aliveCount;
			m_particles[i] = m_particles[m_aliveCount];
			--i;
		}
	}

	m_emitTimer += a_dt;
	EmitParticles();

	for (unsigned int i = 0; i < m_aliveCount; ++i)
	{
		// move particle
		m_particles[i].position += a_dt * m_particles[i].velocity;

		float t = m_particles[i].lifetime / m_particles[i].lifespan;

		// color particle
		m_particles[i].color = glm::mix(m_startColor, m_endColor, t);

		// size particle
		m_particles[i].size = glm::mix(m_startSize, m_endSize, t);
	}
}


void Emitter::UpdateVertexData(vec3 cam_pos, vec3 cam_up)
{
	for (unsigned int i = 0; i < m_aliveCount; ++i)
	{
		mat4 particle_transform(1);

		// create billboard transform
		vec3 to = cam_pos - m_particles[i].position.xyz;

		vec3 f = glm::normalize(to);
		vec3 r = glm::cross(cam_up, f);
		vec3 u = glm::cross(f, r);

		f *= m_particles[i].size;
		r *= m_particles[i].size;
		u *= m_particles[i].size;

		particle_transform[0].xyz = r;
		particle_transform[1].xyz = u;
		particle_transform[2].xyz = f;
		particle_transform[3] = m_particles[i].position;

		m_vertexData[i * 4 + 0].position = particle_transform * vec4(-1, 1, 0, 1);
		m_vertexData[i * 4 + 1].position = particle_transform * vec4(-1, -1, 0, 1);
		m_vertexData[i * 4 + 2].position = particle_transform * vec4(1, -1, 0, 1);
		m_vertexData[i * 4 + 3].position = particle_transform * vec4(1, 1, 0, 1);

		m_vertexData[i * 4 + 0].color = m_particles[i].color;
		m_vertexData[i * 4 + 1].color = m_particles[i].color;
		m_vertexData[i * 4 + 2].color = m_particles[i].color;
		m_vertexData[i * 4 + 3].color = m_particles[i].color;
	}
}


void Emitter::Render()
{
	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers.m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_aliveCount * 4 *
		sizeof(ParticleVertex), m_vertexData);

	// draw particles
	glBindVertexArray(m_buffers.m_VAO);
	glDrawElements(GL_TRIANGLES, m_aliveCount * 6, GL_UNSIGNED_INT, 0);
}