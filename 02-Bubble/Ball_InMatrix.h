#pragma once
#include "Ball.h"
#include "Collider.h"
#include <utility>
#include <list>

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
	bool addNeighbor(posT b);
	bool removeNeighbor(posT b);
	std::vector<posT> getNeighbors();
	void resetNeighbors();
	std::list<Ball_InMatrix*> getDisconnected();

private:
	typedef Collider::lineSegment lineSegment;

	bool _oddRow;
	bool _topRow;
	posT _posInMatrix;
	std::vector<posT> neighbors;
	Collider collider;
};

