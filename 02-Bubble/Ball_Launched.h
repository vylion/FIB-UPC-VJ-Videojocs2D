#pragma once
#include "Ball.h"
#include "Ball_Held.h"
#include "../TileMap.h"

class Ball_Launched :
	public Ball
{
public:
	Ball_Launched(ShaderProgram & shaderProgram, Ball_Held * b, float angle, TileMap *tmap);
	void update(int deltaTime);

	glm::vec2 getSpeed();
	void bounceHorizontal(int deltaTime);
	void bounceVertical(int deltaTime);
	bool movingRight();
	bool movingLeft();
	bool movingUp();
	bool movingDown();

private:
	glm::vec2 _speed;
	TileMap *_tmap;
};

