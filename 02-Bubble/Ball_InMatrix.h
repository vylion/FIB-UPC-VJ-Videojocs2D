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
	typedef std::pair<int, int> posT;

	enum NeighborBalls {
		TOP_LEFT = 0, TOP_RIGHT, RIGHT, LEFT, BOT_RIGHT, BOT_LEFT, OUTSIDE
	};

	Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b);

	bool getOddRow();
	bool getTopRow();
	void setOddRow(const bool &oddRow);
	void setTopRow(const bool &topRow);

	NeighborBalls checkCollision(Ball * b);
	bool addNeighbor(posT b, NeighborBalls pos);
	std::vector<posT> getNeighbors();
	std::list<Ball_InMatrix*> getDisconnected();

private:
	typedef Collider::lineSegment lineSegment;

	bool _oddRow;
	bool _topRow;
	posT _posInMatrix;
	std::vector<posT> neighbors;
	Collider collider;
};

