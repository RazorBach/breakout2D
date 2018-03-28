#include "Particle_generator.h"

ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, const GLuint amount):shader(shader), texture(texture), amount(amount) {
	init();
}


std::unique_ptr<ParticleGenerator> createParticleGenerator(const Shader & shader, const Texture2D & texture, const GLuint amount)
{
	return std::unique_ptr<ParticleGenerator>(new ParticleGenerator(shader, texture, amount));
}

void ParticleGenerator::init() {
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->particleVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->particleVAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

void ParticleGenerator::update(GLfloat dt, GameObject & object, GLuint newParticles, glm::vec2 offset) {
	// Add new 
	for (GLuint i = 0; i < newParticles; ++i) {
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	// Update all
	for (auto& p : particles) {
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f) {	// particle is alive, thus update
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::draw() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.Use();
	for (Particle& particle : particles)
	{
		if (particle.Life > 0.0f)
		{
			shader.SetVector2f("offset", particle.Position);
			shader.SetVector4f("color", particle.Color);
			texture.Bind();
			glBindVertexArray(particleVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int ParticleGenerator::firstUnusedParticle() {
		// Search from last used particle, this will usually return almost instantly
		for (GLuint i = lastUsedParticle; i < amount; ++i) {
			if (particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Otherwise, do a linear search
		for (GLuint i = 0; i < lastUsedParticle; ++i) {
			if (particles[i].Life <= 0.0f) {
				lastUsedParticle = i;
				return i;
			}
		}
		// Override first particle if all others are alive
		lastUsedParticle = 0;
		return 0;
}

void ParticleGenerator::respawnParticle(Particle & particle, const GameObject & object, glm::vec2 offset) {
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.velocity * 0.1f;
}

