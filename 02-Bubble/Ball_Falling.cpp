#include "Ball_Falling.h"

Ball_Falling::Ball_Falling(ShaderProgram & shaderProgram, Ball_InMatrix * b, float angle)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{
	setColor(b->getColor());
	setPosition(b->getPosition());

	float spd_angle = angle;// -float(M_PI);
	float spdx = 0.15f * std::cos(spd_angle);
	float spdy = 0.15f * std::sin(spd_angle);
	_speed = glm::vec2(spdx, spdy);
	_ballSize = b->getSize();
	_sizeShrinked = _ballSize;
}

void Ball_Falling::update(int deltaTime)
{
	glm::vec2 traveledDistance = glm::vec2(_speed.x*deltaTime, _speed.y*deltaTime);
	glm::vec2 finalPosition = glm::vec2(getPosition().x + traveledDistance.x, getPosition().y + traveledDistance.y);
	setPosition(finalPosition);

	if (_sizeShrinked > 0) {
		_sizeShrinked = max(_sizeShrinked - 0.4f, 0.f);
		int size = (int)_sizeShrinked;
		setSize(size);
	}
	
	_renderPos = glm::vec2((float)_ballSize / 2.f) - glm::vec2(getSize() / 2.f);

	_speed += GRAVITY;
}

bool Ball_Falling::dead()
{
	return _sizeShrinked <= 0;
}

void Ball_Falling::render() {
	__super::render(_renderPos);
}
