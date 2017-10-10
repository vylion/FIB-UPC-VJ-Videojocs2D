#pragma once
#include "Ball.h"
class Ball_InMatrix :
	public Ball
{
public:
	Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b);

	bool getOddRow();
	void setOddRow(const bool &oddRow);

	bool checkCollision(Ball * b);


private:
	bool _oddRow;

	void snapToGrid();
};

