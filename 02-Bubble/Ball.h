#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "../Sprite.h"

class Ball
{
public:
	//sizeinspritesheet, spritesheet, shaderprogram
	Ball(const glm::vec2 &size, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram & shaderProgram);

	void init(int color, const glm::vec2 &position);
	void update(int &deltaTime);
	void render();

	int getColor();
	void setColor(int color);

	glm::vec2 getSize();
	void setSize(glm::vec2 &size);

	glm::vec2 getSpritesheetSize();
	Texture * getTexture();

	glm::vec2 getPosition();
	void setPosition(const glm::vec2 &pos);

private:
	glm::vec2 _position, _size, _sizeInSpritesheet;
	int _color;
	Sprite *_sprite;
	Texture *_texture;
};
#endif

