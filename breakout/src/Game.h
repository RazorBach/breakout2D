#ifndef GAME_H
#define GAME_H
#include <glad\glad.h>
#include <GLFW\glfw3.h>
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};
class Game
{
public:
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	Game(GLuint width, GLuint height);
	~Game();
	void Init();
	void Render();
	void Update(GLfloat dt);
	void ProcessInput(GLfloat dt);


private:

};
#endif // !GAME_H
