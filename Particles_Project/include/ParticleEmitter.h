#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size;
	float lifeTime;
	float lifeSpan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 color;
};


class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

protected:
	Particle* mParticles;
	unsigned int mFirstDead;
	unsigned int mMaxParticles;
	unsigned int mVAO, mVBO, mIBO;
	ParticleVertex* mVertexData;

	void Init(unsigned int a_maxParticles,
		unsigned int a_emitRate,
		float a_lifeTimeMin, float a_lifeTimeMax,
		float a_velocityMin, float a_velocityMax,
		float a_startSize, float a_endSize,
		const glm::vec4& a_stratColor, const glm::vec4 a_endColor);

	glm::vec3 mPosition;

	float mEmitTimer;
	float mEmitRate;

	float mLifeSpanMin;
	float mLifeSpanMax;

	float mVelocityMin;
	float mVelocityMax;

	float mStartSize;
	float mEndSize;

	glm::vec4 mStartColor;
	glm::vec4 mEndColor;
};
