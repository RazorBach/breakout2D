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
	if (renderer)
		renderer.reset();
}

void Game::Init() {
	ResourceManager::LoadShader("shader/sprite.vs", "shader/sprite.fs", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.f, -1.f, 1.f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	renderer = createSpriteRenderer(ResourceManager::GetShader("sprite"));
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
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
	ball = std::unique_ptr<BallObject>(new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face")));

	
}

void Game::Render()
{
	if (this->state == GAME_ACTIVE) {
		renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->width, this->height));

		this->levels[this->level].Draw(renderer);
		player->Draw(renderer);
		ball->Draw(renderer);
	}
}

void Game::Update(GLfloat dt)
{

	ball->Move(dt, width);
	DoCollisions();
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
	for (GameObject& box : this->levels[this->level].bricks) {
		if (!box.isDestroyed) {
			if (checkCollision(*ball, box)) {
				if (!box.isSolid)
					box.isDestroyed = GL_TRUE;
			}
		}
	}
}

GLboolean  Game::checkCollision(BallObject & one, GameObject & two)
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
	return glm::length(diff) < one.radius;
}
