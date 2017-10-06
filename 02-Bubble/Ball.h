#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "../Sprite.h"

class Ball
{
public:
	Ball();
	~Ball();

	void init(int color, const glm::vec2 &position, const glm::vec2 &size, Texture *spritesheet, ShaderProgram & shaderProgram);
	void update(int deltaTime);
	void render();
	void renderHeld(float &angle, glm::vec2 &aimerPos, glm::vec2 &aimerSize);
	
	bool checkCollision(Ball &b);
	void launch(float &angle);
	void fall();
	bool isFalling();

	int getColor();

private:
	glm::vec2 _position, _size, _speed;
	int _color;
	bool _falling, _isHeld;
	Sprite *_sprite;
	Texture *_texture;

	void setSpeed(const glm::vec2 &speed);
};
#endif

