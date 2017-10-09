#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "../Sprite.h"

class Ball
{
public:
	//sizeinspritesheet, spritesheet, shaderprogram
	Ball(const glm::vec2 &size, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram & shaderProgram);

	void init(int color, const glm::vec2 &position, bool falling);
	void update(int &deltaTime);
	void render();
	
	bool checkCollision(Ball * b);

	bool isFalling();
	//void fall();

	int getColor();
	void setColor(int color);

	glm::vec2 getSize();
	void setSize(glm::vec2 &size);

	glm::vec2 getSpritesheetSize();

	glm::vec2 getPosition();
	void setPosition(glm::vec2 &pos);

	bool getOddRow();
	void setOddRow(const bool &oddRow);

private:
	glm::vec2 _position, _size, _sizeInSpritesheet;
	int _color;
	bool _falling, _oddRow;
	Sprite *_sprite;
	Texture *_texture;

	//void setSpeed(const glm::vec2 &speed);
};
#endif

