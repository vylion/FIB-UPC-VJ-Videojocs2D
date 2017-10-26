#pragma once
#include "Ball.h"
#include "Collider.h"
#include <utility>
#include <list>
#define POS_NULL posT(-1, -1)

class Ball_InMatrix :
	public Ball
{
public:
	static enum NeighborBalls {
		TOP_LEFT = 0, TOP_RIGHT, RIGHT, LEFT, BOT_RIGHT, BOT_LEFT, OUTSIDE
	};

	typedef std::pair<int, int> posT;
	

	Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b);

	bool getOddRow();
	bool getTopRow();
	void setOddRow(const bool &oddRow);
	void setTopRow(const bool &topRow);

	NeighborBalls checkCollision(Ball * b);
	bool addNeighbor(posT b, int pos);
	std::list<posT> checkNeighbors();
	std::list<Ball_InMatrix*> getDisconnected();

private:
	bool _oddRow;
	bool _topRow;
	posT _posInMatrix;
	std::vector<posT> neighbors;
	Collider collider;
};

