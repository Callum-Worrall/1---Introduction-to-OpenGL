#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "glm_include.h"
#include "Vertex.h"
//Contains particles
//Spawns New Particles
//Updates existing particles
//Renders it's particles


struct Particle
{
	vec4 position;	//change to vertex
	vec4 color;	//change to vertex

	vec4 velocity;

	float size;
	float lifetime;
	float lifespan;
};


struct ParticleVertex
{
	vec4 position;
	vec4 color;
};


class Emitter
{
public:
	Emitter();
	~Emitter();

	//void Initialize(unsigned int a_maxParticles, vec3 a_position,
	//	float a_emitRate,
	//	float a_lifespanMin, float a_lifespanMax,
	//	float a_velocityMin, float a_velocityMax,
	//	float a_startSize, float a_endSize,
	//	vec4 a_startcolor, vec4 a_endcolor);

	//void EmitParticles();

	//void Update(float a_dt, const glm::mat4& a_cameraTransform);

	//void Render();

	void Initialize(unsigned int a_maxParticles, vec3 a_position,
					float a_emitRate,
					float a_lifespanMin, float a_lifespanMax,
					float a_velocityMin, float a_velocityMax,
					float a_startSize, float a_endSize,
					vec4 a_startcolor, vec4 a_endcolor);
	
	void EmitParticles();
	
	void Update(float a_dt);
	void UpdateVertexData(vec3 cam_pos, vec3 cam_up);
	
	void Render();

private:

	unsigned int m_firstDead;

	Particle *m_particles;
	unsigned int m_maxParticles;
	unsigned int m_aliveCount;

	//OpenGL Data
	OpenGLData m_buffers;

	ParticleVertex* m_vertexData;
	unsigned int* m_indexData;

	vec4 m_position;

	//how long the particles are emitted for
	float m_emitTimer;

	//how often the particles emit, while emitting
	float m_emitRate;

	//how long the particles will last for
	float m_lifespanMin;
	float m_lifespanMax;

	//How fast they move away
	float m_velocityMin;
	float m_velocityMax;

	//scale size of particle
	float m_startSize;
	float m_endSize;

	unsigned int m_program_ID;

	vec4 m_startColor;
	vec4 m_endColor;
};

#endif