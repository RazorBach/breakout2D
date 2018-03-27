#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include "Game_object.h"

class BallObject : public GameObject {
public:
	// ball state
	GLfloat radius;
	GLboolean stuck;

	// powerups
	GLboolean sticky, passThrough;

	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 Move(GLfloat dt, GLuint windowWidth);
	void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif //BALL_OBJECT_H