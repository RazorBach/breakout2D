#include "Ball_object.h"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(GL_TRUE), sticky(GL_FALSE), passThrough(GL_FALSE) {}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite) : GameObject(pos, glm::vec2(
	radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(GL_TRUE), sticky(GL_FALSE),
	passThrough(GL_FALSE) {}

glm::vec2 BallObject::Move(GLfloat dt, GLuint windowWidth)
{
	if (!this->stuck) {
		this->position += this->velocity * dt;
		if (this->position.x <= 0.f) {
			this->velocity.x *= -1;
			this->position.x = 0.f;
		}
		else if (this->position.x + this->size.x >= windowWidth) {
			this->velocity.x *= -1;
			this->position.x = windowWidth - this->size.x;
		}
		if (this->position.y <= 0.f) {
			this->velocity.y *= -1;
			this->position.y = 0.f;
		}
	}
	return this->position;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}
