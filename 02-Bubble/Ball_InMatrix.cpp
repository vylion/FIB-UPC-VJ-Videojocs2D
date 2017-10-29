#include "Ball_InMatrix.h"

Ball_InMatrix::Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{

	setColor(b->getColor());
	//snapToGrid();

	neighbors = std::vector<posT>(6, POS_NULL);
}

bool Ball_InMatrix::getOddRow()
{
	return _oddRow;
}

bool Ball_InMatrix::getTopRow()
{
	return _topRow;
}

void Ball_InMatrix::setOddRow(const bool & oddRow)
{
	_oddRow = oddRow;
	if (_oddRow) {
		//Will add half the sprite of distance if odd
		glm::vec2 oddPos = glm::vec2(getPosition().x + getSize() / 2 * _oddRow, getPosition().y);
		setPosition(oddPos);
	}
}

void Ball_InMatrix::setTopRow(const bool & topRow)
{
	_topRow = topRow;
}

Ball_InMatrix::NeighborBalls Ball_InMatrix::checkCollision(Ball * b)
{
	std::vector<glm::vec2> ownPoints = collisionPoints();
	std::vector<glm::vec2> ballPoints = b->collisionPoints();

	lineSegment backslash = lineSegment(ballPoints[5], ballPoints[2]);	// Diagonal "\" 
	lineSegment slash = lineSegment(ballPoints[4], ballPoints[1]);		// Diagonal "/"
	lineSegment dash = lineSegment(ballPoints[3], ballPoints[0]);		// Diagonal "-"

	lineSegment sideBR = lineSegment(ownPoints[5], ownPoints[0]);
	lineSegment sideBot = lineSegment(ownPoints[4], ownPoints[5]);
	lineSegment sideBL = lineSegment(ownPoints[3], ownPoints[4]);

	lineSegment sideTR = lineSegment(ownPoints[1], ownPoints[0]);
	lineSegment sideTop = lineSegment(ownPoints[2], ownPoints[1]);
	lineSegment sideTL = lineSegment(ownPoints[3], ownPoints[2]);

	if (collider.lineCollision(sideBR, backslash)) {
		return BOT_RIGHT;
	}
	else if (collider.lineCollision(sideBot, backslash)) {
		return BOT_RIGHT;
	}
	else if (collider.lineCollision(sideBot, slash)) {
		return BOT_LEFT;
	}
	else if (collider.lineCollision(sideBL, slash)) {
		return BOT_LEFT;
	}
	else if (collider.lineCollision(sideBR, dash)) {
		return BOT_RIGHT;
	}
	else if (collider.lineCollision(sideTR, dash)) {
		return RIGHT;
	}
	else if (collider.lineCollision(sideBL, dash)) {
		return BOT_LEFT;
	}
	else if (collider.lineCollision(sideTL, dash)) {
		return LEFT;
	}
	else if (collider.lineCollision(sideTL, backslash)) {
		return TOP_LEFT;
	}
	else if (collider.lineCollision(sideTop, backslash)) {
		return TOP_LEFT;
	}
	else if (collider.lineCollision(sideTR, slash)) {
		return TOP_RIGHT;
	}
	else if (collider.lineCollision(sideTop, slash)) {
		return TOP_RIGHT;
	}

	else
		return OUTSIDE;
}

bool Ball_InMatrix::addNeighbor(Ball_InMatrix::posT b, int pos)
{
	if (pos < 6 && pos >= 0) {
		neighbors[pos] = b;
		return true;
	}
	return false;
}

list<Ball_InMatrix::posT> Ball_InMatrix::checkNeighbors()
{
	list<posT> group = list<posT>();

	for (int i = TOP_LEFT; i < OUTSIDE; ++i) {
		if (neighbors[i] != POS_NULL) {
			//check if same color, among colors, or bomb
			group.push_back(neighbors[i]);
		}
		return group;
	}
}

list<Ball_InMatrix*> Ball_InMatrix::getDisconnected()
{
	return list<Ball_InMatrix*>();
}

/*
void Ball_InMatrix::snapToGrid() {
	glm::vec2 closestValidPos = getPosition();
	// pos/size to get the lower snap point
	// %size to know at what above point we are right now
	// /size/2 to know if we are on the first or second half
	// *size to snap to correct point
	closestValidPos.x = float(int(closestValidPos.x) / getSize() + ((int(closestValidPos.x) % getSize()) / (getSize() / 2)) * getSize());
	closestValidPos.y = float(int(closestValidPos.y) / getSize() + ((int(closestValidPos.y) % getSize()) / (getSize() / 2)) * getSize());
	setPosition(closestValidPos);
}*/