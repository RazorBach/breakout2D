#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include "Shader.h"
#include "Texture.h"
#include <memory>

class SpriteRenderer
{
public:
	friend std::shared_ptr<SpriteRenderer> createSpriteRenderer(Shader &shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	Shader shader;
	GLuint quadVAO;

	SpriteRenderer(Shader &shader);
	void initRenderData();
};

#endif // !SPRITE_RENDERER_H
