#include "Ball_Held.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>


Ball_Held::Ball_Held(ShaderProgram & shaderProgram, Ball * b)
	:Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{

	setColor(b->getColor());
	_originalSize = b->getSize();
	setSize(_originalSize);

}

void Ball_Held::initHeldPosition(const glm::vec2 position, float &angle) {
	//Angle rotated 1/4 of circumference clockwise to calculate position correctly
	_angle = angle - float(M_PI / 2);
	_shootingPosition = position;
	//Update position
	setPosition(_shootingPosition);
}

void Ball_Held::updateShooting(int deltaTime, int maxTime) {
	int size;
	glm::vec2 pos;
	//Increase regularly from 0 to normal ball size
	size = min(_originalSize, (int)((float)deltaTime/(float)maxTime*_originalSize));

	pos = _shootingPosition + glm::vec2((float)_originalSize/2.f) - glm::vec2((float)deltaTime / (float)maxTime * _originalSize /2.f);

	setSize(size);
	setPosition(pos);
}

float Ball_Held::getAngle()
{
	return _angle;
}

