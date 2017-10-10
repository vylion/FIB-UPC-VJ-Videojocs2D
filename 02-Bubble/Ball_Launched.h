#pragma once
#include "Ball.h"
#include "Ball_Held.h"

class Ball_Launched :
	public Ball
{
public:
	Ball_Launched(ShaderProgram & shaderProgram, Ball_Held * b, float angle, glm::ivec2 &minCoords, glm::ivec2 &fieldLimit);
	void update(int deltaTime);
private:
	glm::vec2 _speed, _minCoords, _fieldLimits;
};

