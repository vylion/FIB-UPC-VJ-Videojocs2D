#include "Ball.h"
#define _USE_MATH_DEFINES
#include <math.h>

Ball::Ball(const int &size, const glm::vec2 & sizeInSpritesheet,  Texture *spritesheet, ShaderProgram & shaderProgram)
{
	_size = size;
	_sizeInSpritesheet = sizeInSpritesheet;
	_texture = spritesheet;
	_sprite = Sprite::createSprite(glm::vec2(size, size), sizeInSpritesheet, spritesheet, &shaderProgram);
}

void Ball::init(int color, const glm::vec2 & position)
{
	_color = color;
	//Set texture according to color code (x = 1.f sheet size / 8 different colors * color code)
	_sprite->setTexturePosition(glm::vec2(1.f/8 * color, 0.f));
	setPosition(position);
}

void Ball::update(int & deltaTime)
{
	/*if (_falling) {
		_position = glm::vec2(_position.x + _speed.x*deltaTime / 200, _position.y + _speed.y*deltaTime / 200);
		_speed = glm::vec2(_speed.x*0.95f, _speed.y*1.2f);
	}*/
}

void Ball::render()
{
	_sprite->render();
}

int Ball::getColor()
{
	return _color;
}

void Ball::setColor(int color)
{
	_color = color;
	_sprite->setTexturePosition(glm::vec2(1.f / 8 * color, 0.f));
}

int Ball::getSize()
{
	return _size;
}

void Ball::setSize(int & size)
{
	_size = size;
	_sprite->setSize(glm::vec2(size, size));
}

glm::vec2 Ball::getSpritesheetSize()
{
	return _sizeInSpritesheet;
}

Texture * Ball::getTexture()
{

	return _texture;
}

glm::vec2 Ball::getPosition()
{
	return _position;
}

void Ball::setPosition(const glm::vec2 & pos)
{
	_position = pos;
	_sprite->setPosition(_position);
}

vector<glm::vec2> Ball::collisionPoints()
{
	float r = _size / 2;
	glm::vec2 p;
	vector<glm::vec2> points = vector<glm::vec2>();

	for (int i = 0; i < 6; ++i) {
		p = glm::vec2(r*sin(i*(30 * M_PI / 180)), r*cos(i*(30 * M_PI / 180)));

		points.push_back(p + _position);
	}

	return points;
}
