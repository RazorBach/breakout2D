#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glad\glad.h>
#include "Game_object.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator {
public:
	friend std::unique_ptr<ParticleGenerator> createParticleGenerator(const Shader& shader, const Texture2D& texture, const GLuint amount);

	void update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset);
	void draw();
private:

	ParticleGenerator(const Shader& shader, const Texture2D& texture, const GLuint amount);
	void init();
	Shader shader;
	Texture2D texture;
	GLuint particleVAO;

	GLuint amount;
	std::vector<Particle> particles;
	int lastUsedParticle = 0;

	int firstUnusedParticle();
	void respawnParticle(Particle &particle, const GameObject &object, glm::vec2 offset);
};


#endif