#include "Game_object.h"


GameObject::GameObject()
	: position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), isSolid(false), isDestroyed(false), Sprite() { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: position(pos), size(size), velocity(velocity), color(color), rotation(0.0f), Sprite(sprite), isSolid(false), isDestroyed(false) { }

void GameObject::Draw(std::shared_ptr<SpriteRenderer> renderer)
{
	renderer->DrawSprite(this->Sprite, this->position, this->size, this->rotation, this->color);
}