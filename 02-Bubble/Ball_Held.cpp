#include "Ball_Held.h"
#define _USE_MATH_DEFINES
#include <math.h>


Ball_Held::Ball_Held(ShaderProgram & shaderProgram, Ball * b)
	:Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{

	setColor(b->getColor());
	int size = b->getSize();
	setSize(size);

}

void Ball_Held::initHeldPosition(const glm::vec2 &aimerPos, const glm::vec2 &aimerSize) {
	_aimerPos = aimerPos;
	_aimerSize = aimerSize;
}

void Ball_Held::update(int deltaTime, float &angle) {
	//Angle rotated 1/4 of circumference clockwise to calculate position correctly
	_angle = angle - float(M_PI/2);
	//Initial position at the middle of aimer
	glm::vec2 position = _aimerPos + glm::vec2(0.f, _aimerSize.y / 2);
	//Add position relative to angle
	position.x += _aimerSize.x * cos(_angle);
	position.y += _aimerSize.y / 4 * sin(_angle);
	//Update position and skype
	setPosition(position);
	//Animations (soon(tm(?)))
	//Ball::update(deltaTime);
}

float Ball_Held::getAngle()
{
	return _angle;
}

