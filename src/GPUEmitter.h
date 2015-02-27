#ifndef __GPU_EMITTER_H__
#define __GPU_EMITTER_H__

#include "glm_include.h"



struct GPUParticle
{
	GPUParticle() : lifetime(1), lifespan(0) {}

	vec3 position;
	vec3 velocity;

	float lifetime;
	float lifespan;
};


class GPUPointEmitter
{
public:

	GPUPointEmitter();
	~GPUPointEmitter();

	void Initialize(unsigned int a_maxParticles,
		float a_lifespanMin, float a_lifespanMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const vec4 &a_startColour,
		const vec4 &a_endColour);

	void Draw(
		float a_time,
		const mat4 &a_cameraTransform,
		const mat4 &a_projectionView);

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();


	GPUParticle* m_particles;
	unsigned int m_maxParticles;


	vec3 m_position;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	vec4 m_startColour;
	vec4 m_endColour;

protected:



	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	unsigned int m_drawShader;
	unsigned int m_updateShader;

	float m_lastDrawTime;
};


#endif //__GPU_EMITTER_H__