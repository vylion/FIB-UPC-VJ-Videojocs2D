#pragma once
#include "Ball.h"
#include <vector>

class Ball_InMatrix :
	public Ball
{
public:
	static enum NeighborBalls {
		TOP_LEFT = 0, TOP_RIGHT, RIGHT, BOT_RIGHT, BOT_LEFT, LEFT
	};

	Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b);

	bool getOddRow();
	bool getTopRow();
	void setOddRow(const bool &oddRow);
	void setTopRow(const bool &topRow);

	bool checkCollision(Ball * b);
	bool addNeighbor(Ball_InMatrix *b, int pos);
	vector<Ball_InMatrix*> getDisconnected();

private:
	bool _oddRow;
	bool _topRow;
	std::vector<Ball_InMatrix*> neighbors;
};

