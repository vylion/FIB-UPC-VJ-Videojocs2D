#include "Ball_Launched.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

Ball_Launched::Ball_Launched(	Texture * spritesheet,
								ShaderProgram & shaderProgram,
								Ball * b,
								float &angle,
								glm::vec2 &fieldLimits)
	: Ball(b->getSize(), b->getSpritesheetSize(), spritesheet, shaderProgram), _fieldLimits(fieldLimits)
{
	setPosition(b->getPosition());

	float spd_angle = angle + float(M_PI);
	float spdx = 2 * std::cos(spd_angle);
	float spdy = 2 * std::sin(spd_angle);
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
	if (finalPosition.x < 0) {
		//New X position is going to be as much distance above the limit as it went below the limit	
		finalPosition.x = 0 - finalPosition.x;
		_speed.x = -_speed.x;
	}
	else if (finalPosition.x > _fieldLimits.x) {
		//New X position is going to be as much distance below the limit as it went above the limit
		finalPosition.x = _fieldLimits.x - finalPosition.x;
		_speed.x = -_speed.x;
	}
	setPosition(finalPosition);
}
