#include "Ball.h"

Ball::Ball(const glm::vec2 &size, const glm::vec2 & sizeInSpritesheet,  Texture *spritesheet, ShaderProgram & shaderProgram)
{
	_size = size;
	_sizeInSpritesheet = sizeInSpritesheet;
	_sprite = Sprite::createSprite(size, sizeInSpritesheet, spritesheet, &shaderProgram);
	_falling = false;
}

void Ball::init(int color, const glm::vec2 & position, bool falling)
{
	_color = color;
	//Set texture according to color code (x = 1.f sheet size / 8 different colors * color code)
	_sprite->setTexturePosition(glm::vec2(1.f/8 * color, 0.f));
	_position = position;
	_falling = falling;
	_speed = glm::vec2(0.f, 0.f);
}

void Ball::update(int & deltaTime)
{
	if (_falling) {
		_position = glm::vec2(_position.x + _speed.x*deltaTime / 200, _position.y + _speed.y*deltaTime / 200);
		_speed = glm::vec2(_speed.x*0.95f, _speed.y*1.2f);
	}
}

void Ball::render()
{
	_sprite->render();
}

bool Ball::checkCollision(Ball * b)
{
	return false;
}

void Ball::launch(float & angle)
{
	//Restore original size
	_sprite->setSize(_size);
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
	return _color;
}

void Ball::setColor(int color)
{
	_color = color;
}

glm::vec2 Ball::getSize()
{
	return _size;
}

void Ball::setSize(glm::vec2 & size)
{
	_size = size;
}

glm::vec2 Ball::getSpritesheetSize()
{
	return _sizeInSpritesheet;
}

glm::vec2 Ball::getPosition()
{
	return _position;
}

void Ball::setPosition(glm::vec2 & pos)
{
	_position = pos;
	glm::vec2 oddPos = glm::vec2(_position.x + _size.x / 2 * _oddRow, _position.y);
	_sprite->setPosition(oddPos);
}

bool Ball::getOddRow()
{
	return _oddRow;
}

void Ball::setOddRow(const bool & oddRow)
{
	_oddRow = oddRow;
	//Will add half the sprite of distance if odd
	glm::vec2 oddPos = glm::vec2(_position.x + _size.x / 2 * _oddRow, _position.y);

	_sprite->setPosition(oddPos);
}

void Ball::setSpeed(const glm::vec2 & speed)
{
	_speed = speed;
}
