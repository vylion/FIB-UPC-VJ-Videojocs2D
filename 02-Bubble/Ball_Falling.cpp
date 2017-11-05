#include "Ball_Falling.h"

Ball_Falling::Ball_Falling(ShaderProgram & shaderProgram, Ball_InMatrix * b, glm::vec2 speed, glm::vec2 minRenderCoords)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{
	init(b->getColor(), b->getPosition(), minRenderCoords);

	_speed = speed;
	_ballSize = b->getSize();
	_sizeShrinked = (float)_ballSize;
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
