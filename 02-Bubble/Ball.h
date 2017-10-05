#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "../Sprite.h"

class Ball
{
public:
	Ball();
	~Ball();

	void init(int color, const glm::vec2 &position, const glm::vec2 &size);
	void update(int deltaTime);
	void render();
	
	bool checkCollision(Ball &b);
	void fall();
	bool isFalling();

	int getColor();

private:
	glm::vec2 _position, _size, _speed;
	int _color;
	bool _falling;
	Sprite *_sprite;
};
#endif

