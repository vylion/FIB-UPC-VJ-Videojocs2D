#include "Ball_Launched.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

Ball_Launched::Ball_Launched(	ShaderProgram & shaderProgram,
								Ball_Held * b,
								float angle,
								glm::ivec2 &minCoords,
								glm::ivec2 &fieldLimits)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{
	setColor(b->getColor());
	setPosition(b->getPosition());

	_fieldLimits = glm::vec2(float((fieldLimits.x-1)*16.f), float(fieldLimits.y*16.f));
	_minCoords = glm::vec2(float(minCoords.x), float(minCoords.y));

	float spd_angle = angle;// -float(M_PI);
	float spdx = 0.2 * std::cos(spd_angle);
	float spdy = 0.2 * std::sin(spd_angle);
	_speed = glm::vec2(spdx, spdy);
}

void Ball_Launched::update(int deltaTime)
{
	glm::vec2 traveledDistance = glm::vec2(_speed.x*deltaTime, _speed.y*deltaTime);

	glm::vec2 finalPosition = glm::vec2(getPosition().x + traveledDistance.x, getPosition().y + traveledDistance.y);
	/*
	If ball is out of horizontal bounds we will recalculate position with traveled
	total distance and reverse X speed.
	*/

	/*
	glm::vec2 finalPosition = getPosition();

	if (finalPosition.x < _minCoords.x) {
	//New X position is going to be as much distance above the limit as it went below the limit
	finalPosition.x += _minCoords.x - finalPosition.x;
	_speed.x = -_speed.x;
	}
	else if (finalPosition.x > _minCoords.x + _fieldLimits.x) {
	//New X position is going to be as much distance below the limit as it went above the limit
	finalPosition.x += (_minCoords.x + _fieldLimits.x) - finalPosition.x;
	_speed.x = -_speed.x;
	}
	setPosition(finalPosition);
	*/

	setPosition(finalPosition);
}

glm::vec2 Ball_Launched::getSpeed()
{
	return _speed;
}

void Ball_Launched::bounceHorizontal(int deltaTime)
{
	setPosition(glm::vec2(getPosition().x - _speed.x*deltaTime, getPosition().y));
	_speed.x = -_speed.x;
}

void Ball_Launched::bounceVertical(int deltaTime)
{
	setPosition(glm::vec2(getPosition().x, getPosition().y - _speed.y*deltaTime));
	_speed.y = -_speed.y;
}

bool Ball_Launched::movingRight()
{
	return _speed.x > 0;
}

bool Ball_Launched::movingLeft()
{
	return _speed.x < 0;
}

bool Ball_Launched::movingUp()
{
	return _speed.y < 0;
}

bool Ball_Launched::movingDown()
{
	return _speed.y > 0;
}