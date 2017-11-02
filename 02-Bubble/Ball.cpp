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

void Ball::init(int color, const glm::vec2 & position, glm::vec2 minRenderCoords)
{
	setColor(color);

	_minRenderCoords = minRenderCoords;
	setPosition(position);
}

void Ball::update(int & deltaTime)
{
	
}

void Ball::render()
{
	_sprite->render();
}

unsigned int Ball::getColor()
{
	return _color;
}

void Ball::setColor(int color)
{
	_color = color;
	//Set texture according to color code (x = ball size * color code, y = )
	_sprite->setTexturePosition(
		_sizeInSpritesheet / _texture->getSize() *
		//spritesize * color % 8 to get the column, spritesize * 4 * (color/8) to get the row
		//Rows [1..3] saved for sprite animations, so row will be either 0 or 4
		glm::vec2((float)(color % 8), 4.f * (color / 8))
	);
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
	_sprite->setPosition(_position + _minRenderCoords);
}

vector<glm::vec2> Ball::collisionPoints()
{
	float r = float(_size) / 2.f;
	glm::vec2 p;
	vector<glm::vec2> points = vector<glm::vec2>();

	for (int i = 0; i < 6; ++i) {
		p = glm::vec2(r*sin((30 + i * 60 * M_PI) / 180), r*cos((30 + i * 60) * M_PI / 180));

		points.push_back(p + _position);
	}

	return points;
}
