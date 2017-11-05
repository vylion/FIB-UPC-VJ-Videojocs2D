#pragma once
#include <algorithm>
#include "Ball.h"
#include "Ball_InMatrix.h"
#include "../TileMap.h"

#define GRAVITY glm::vec2(0, 0.01f)

class Ball_Falling :
	public Ball
{
public:
	Ball_Falling(ShaderProgram & shaderProgram, Ball_InMatrix * b, glm::vec2 speed, glm::vec2 minRenderCoords);
	void update(int deltaTime);
	void render();

	bool dead();

	//glm::vec2 getSpeed();

private:
	glm::vec2 _speed;
	int _ballSize;
	float _sizeShrinked;
	glm::vec2 _renderPos;
};

