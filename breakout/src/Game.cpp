#include "Game.h"
#include "Sprite_renderer.h"
#include "Resource_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Game::Game(GLuint mwidth, GLuint mheight):state(GAME_ACTIVE), keys(), width(mwidth), height(mheight)
{
}

Game::~Game()
{
}

void Game::Init() {
	ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("shader/particle.vs", "shader/particle.fs", nullptr, "particle");

	glm::mat4 projection = glm::ortho(0.f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.f, -1.f, 1.f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("image", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");



	renderer = createSpriteRenderer(ResourceManager::GetShader("sprite"));
	particleGenerator = createParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);

	// Load levels
	GameLevel one, two, three, four; 
	one.Load("levels/level1.lvl", this->width, this->height * 0.5);
	two.Load("levels/level2.lvl", this->width, this->height * 0.5);
	three.Load("levels/level3.lvl", this->width, this->height * 0.5);
	four.Load("levels/level4.lvl", this->width, this->height * 0.5);
	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);
	this->level = 0;

	glm::vec2 playerPos = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
	player = std::unique_ptr<GameObject>(new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle")));

	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	ball = std::unique_ptr<BallObject>(new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face")));

	
}

void Game::Render()
{
	if (this->state == GAME_ACTIVE) {
		renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height));

		this->levels[this->level].Draw(renderer);
		player->Draw(renderer);
		particleGenerator->draw();
		ball->Draw(renderer);
	}
}

void Game::Update(GLfloat dt)
{
	ball->Move(dt, width);
	particleGenerator->update(dt, *ball, 2, glm::vec2(ball->radius / 2));
	DoCollisions();
	if (ball->position.y >= height) {
		ResetLevel();
		ResetPlayer();
	}
}

void Game::ResetLevel() {
	std::string currentLevelFileName;
	switch (this->level) {
	case 0:
		currentLevelFileName = "levels/level1.lvl";
		break;
	case 1:
		currentLevelFileName = "levels/level2.lvl";
		break;
	case 2:
		currentLevelFileName = "levels/level3.lvl";
		break;
	case 3:
		currentLevelFileName = "levels/level4.lvl";
		break;
	default:
		break;
	}

	levels[level].Load(currentLevelFileName.c_str(), width, height / 2);

	//powerUps.clear();
	//lives = 3;
}

void Game::ResetPlayer() {
	player->size = PLAYER_SIZE;
	player->velocity = glm::vec2(PLAYER_VELOCITY, 0.0f);
	ball->Reset(player->position + glm::vec2(player->size.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)),
		INITIAL_BALL_VELOCITY);
}

void Game::ProcessInput(GLfloat dt)
{
	if (this->state == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->keys[GLFW_KEY_LEFT])
		{
			if (player->position.x >= 0) {
				player->position.x -= velocity;
				if (ball->stuck)
					ball->position.x -= velocity;
			}
		}
		if (this->keys[GLFW_KEY_RIGHT])
		{
			if (player->position.x <= this->width - player->size.x) {
				player->position.x += velocity;
				if (ball->stuck)
					ball->position.x += velocity;
			}
		}
		if (this->keys[GLFW_KEY_SPACE])
			ball->stuck = false;
	}
}

void Game::DoCollisions()
{
	//Óëµ²°åÏà×²
	Collision result = CheckCollision(*ball, *player);
	if (!ball->stuck && std::get<0>(result)) {
		GLfloat center = player->position.x + player->size.x / 2;
		GLfloat distance = (ball->position.x + ball->radius) - center;
		GLfloat percent = distance / (player->size.x / 2);
		GLfloat strength = 4.f;
		glm::vec2 oldv = ball->velocity;
		ball->velocity.x = INITIAL_BALL_VELOCITY.x * percent * strength;
		ball->velocity.y = -abs(ball->velocity.y);
		ball->velocity = glm::normalize(ball->velocity) * glm::length(oldv);
	}
	//Óë×©¿éÏà×²
	for (GameObject& box : this->levels[this->level].bricks) {
		if (!box.isDestroyed) {
			Collision collision = CheckCollision(*ball, box);
			if (std::get<0>(collision)) {
				if (!box.isSolid)
					box.isDestroyed = GL_TRUE;
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vec = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) {
					//horizontal
					ball->velocity.x *= -1;
					//penetration ÉøÍ¸
					GLfloat penetration = ball->radius - std::abs(diff_vec.x);
					if (dir == LEFT)
						ball->position.x += penetration;
					else
						ball->position.x -= penetration;
				}
				else {
					//vertical
					ball->velocity.y *= -1;
					GLfloat penetration = ball->radius - std::abs(diff_vec.y);
					if (dir == UP)
						ball->position.y -= penetration;
					else
						ball->position.y += penetration;

				}
			}
		}
	}
}
GLboolean Game::CheckCollision(GameObject &one, GameObject &two) {
	// x-axis
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;
	// y-axis
	bool collisionY = one.position.y + one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;

	return (GLboolean)(collisionX && collisionY);
}

Collision  Game::CheckCollision(BallObject &one, GameObject & two)
{
	glm::vec2 center(one.position + one.radius);
	//AABB
	glm::vec2 aabb_half(two.size.x / 2, two.size.y / 2);
	glm::vec2 aabb_center(two.position.x + aabb_half.x, two.position.y + aabb_half.y);

	//center to center
	glm::vec2 c2c = center - aabb_center;
	//AABB center to closest point on border
	glm::vec2 c2b = glm::clamp(c2c, -aabb_half, aabb_half);
	//closest point on border
	glm::vec2 closest = aabb_center + c2b;
	glm::vec2 diff = closest - center;
	if (glm::length(diff) <= one.radius)
		return std::make_tuple(GL_TRUE, VectorDirection(diff), diff);
	return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	GLfloat max = 0.0f;
	GLint bestMatch = -1;
	for (GLint i = 0; i < 4; i++) {
		GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			bestMatch = i;
		}
	}
	return (Direction)bestMatch;
}
