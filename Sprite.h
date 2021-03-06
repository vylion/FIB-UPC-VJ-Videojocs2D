#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	//static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &texSize, const glm::vec2 &texPos, Texture *spritesheet, ShaderProgram *program);
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	//Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, const glm::vec2 &posInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	void update(int deltaTime);
	void render() const;
	void render(glm::vec2 displacement);
	void render(float angle) const;
	void render(float angle, const glm::vec2 rotationAxisRatio) const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	int animation() const;
	
	void setPosition(const glm::vec2 &pos);
	void setTexturePosition(const glm::vec2 &displacement);
	void setSize(const glm::vec2 &size);

private:
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position, initialSize, size;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	vector<AnimKeyframes> animations;

};


#endif // _SPRITE_INCLUDE

