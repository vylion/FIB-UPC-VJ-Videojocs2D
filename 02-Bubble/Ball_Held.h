#pragma once
#include "Ball.h"

class Ball_Held :
	public Ball
{
public:
	Ball_Held(ShaderProgram & shaderProgram, Ball * b);
	void init(const glm::vec2 &aimerPos, const glm::vec2 &aimerSize);
	void update(int deltaTime, float &angle);

private:
	glm::vec2 _aimerPos, _aimerSize;
	float _angle;
};