#pragma once
#include "Ball.h"
class Ball_Launched :
	public Ball
{
public:
	Ball_Launched(Texture *spritesheet, ShaderProgram & shaderProgram, Ball * b, float &angle, glm::vec2 &fieldLimit);
	void update(int deltaTime);
private:
	glm::vec2 _speed, _fieldLimits;
};

