#ifndef GAME_H
#define GAME_H
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Sprite_renderer.h"
#include "Game_level.h"
#include "Ball_object.h"
#include "Particle_generator.h"

enum Direction {
	UP, RIGHT, DOWN, LEFT
};

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// player
const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


class Game
{
public:
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	std::vector<GameLevel> levels;
	GLuint level;
	Game(GLuint width, GLuint height);
	~Game();
	void Init();
	void Render();
	void Update(GLfloat dt);
	void ResetLevel();
	void ResetPlayer();
	void ProcessInput(GLfloat dt);
	void DoCollisions();

	// game objects
	std::unique_ptr<GameObject> player;
	//todo 
	std::unique_ptr<BallObject> ball;

private:
	std::shared_ptr<SpriteRenderer> renderer;
	std::unique_ptr<ParticleGenerator> particleGenerator;

	GLboolean CheckCollision(GameObject &one, GameObject &two);
	Collision CheckCollision(BallObject &one, GameObject &two);
	Direction VectorDirection(glm::vec2 target);
};
#endif // !GAME_H
