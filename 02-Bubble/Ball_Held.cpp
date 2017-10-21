#include "Ball_Held.h"
#define _USE_MATH_DEFINES
#include <math.h>


Ball_Held::Ball_Held(ShaderProgram & shaderProgram, Ball * b)
	:Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{

	setColor(b->getColor());
	int size = b->getSize();
	setSize(size);
	//setSize(glm::vec2(b->getSize().x,b->getSize().y));

}

void Ball_Held::init(const glm::vec2 &aimerPos, const glm::vec2 &aimerSize) {
	_aimerPos = aimerPos;
	_aimerSize = aimerSize;
}

void Ball_Held::update(int deltaTime, float &angle) {
	_angle = angle + float(3*M_PI/2);
	glm::vec2 position = glm::vec2(_aimerPos.x, _aimerPos.y + _aimerSize.y/2);
	position.x += _aimerSize.x * cos(_angle);
	position.y += _aimerSize.y / 4 * sin(_angle);
	setPosition(position);
	Ball::update(deltaTime);
}

float Ball_Held::getAngle()
{
	return _angle;
}

