#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"


// static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. 
class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	static Shader   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	static Shader   GetShader(std::string name);
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	static Texture2D GetTexture(std::string name);
	static void      Clear();
private:
	ResourceManager() { }
	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	// Loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif