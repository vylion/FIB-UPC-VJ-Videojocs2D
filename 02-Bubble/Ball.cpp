#include "Ball.h"

Ball::Ball()
{
}


Ball::~Ball()
{
}

void Ball::init(int color, const glm::vec2 & position, const glm::vec2 & size, Texture *spritesheet, ShaderProgram & shaderProgram)
{
	_sprite = Sprite::createSprite(glm::vec2(32,32), size, glm::vec2(16.f*color,0.f), spritesheet, &shaderProgram);
	_isHeld = false;
	_falling = false;
	_color = color;
	_position = position;
	_size = size;
	_speed = glm::vec2(0.f, 0.f);
}

void Ball::update(int deltaTime)
{
	if (_falling) {
		_position = glm::vec2(_position.x + _speed.x*deltaTime / 200, _position.y + _speed.y*deltaTime / 200);
		_speed = glm::vec2(_speed.x*0.95f, _speed.y*1.2f);
	}
}

void Ball::render()
{
	//_sprite->render();
}

void Ball::renderHeld(float & angle, glm::vec2 & aimerPos, glm::vec2 & aimerSize)
{

	_sprite->render();
}

bool Ball::checkCollision(Ball & b)
{
	return false;
}

void Ball::launch(float & angle)
{
}

void Ball::fall()
{
}

bool Ball::isFalling()
{
	return _falling;
}

int Ball::getColor()
{
	return 0;
}

void Ball::setSpeed(const glm::vec2 & speed)
{
}
