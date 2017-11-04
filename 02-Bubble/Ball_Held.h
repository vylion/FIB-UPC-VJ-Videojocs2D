#pragma once
#include "Ball.h"

class Ball_Held :
	public Ball
{
public:
	Ball_Held(ShaderProgram & shaderProgram, Ball * b);
	void initHeldPosition(const glm::vec2 position, float &angle);
	void updateShooting(int deltaTime, int maxTime);
	void updateSwapping(int deltaTime, int maxTime, bool toSwapped);

	float getAngle();

private:
	glm::vec2 _aimerPos, _aimerSize, _minRenderCoords;
	glm::vec2 _shootingPosition;
	int _originalSize;
	float _angle;
};