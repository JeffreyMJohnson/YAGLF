#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	: mParticles(nullptr),
	mFirstDead(0),
	mMaxParticles(0),
	mPosition(0, 0, 0),
	mVAO(0), mVBO(0), mIBO(0),
	mVertexData(nullptr){}

ParticleEmitter::~ParticleEmitter()
{
	delete[] mParticles;
	delete[] mVertexData;

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mIBO);
}

void ParticleEmitter::Init(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifeTimeMin, float a_lifeTimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4 & a_startColor, const glm::vec4 a_endColor)
{
	//set up timer
	mEmitTimer = 0;
	mEmitRate = 1.0f / a_emitRate;

	mStartColor = a_startColor;
	mEndColor = a_endColor;
	mStartSize = a_startSize;
	mEndSize = a_endSize;
	mVelocityMin = a_velocityMin;
	mVelocityMax = a_velocityMax;
	mLifeSpanMin = a_lifeTimeMin;
	mLifeSpanMax = a_lifeTimeMax;
	mMaxParticles = a_maxParticles;
	
	mParticles = new Particle[mMaxParticles];
	mFirstDead = 0;

	mVertexData = new ParticleVertex[mMaxParticles * 4];

	// create the index buffeer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	unsigned int* indexData = new unsigned int[mMaxParticles * 6];
	for (unsigned int i = 0; i < mMaxParticles; ++i) {
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 *
		sizeof(ParticleVertex), mVertexData,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMaxParticles * 6 *
		sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indexData;
}
