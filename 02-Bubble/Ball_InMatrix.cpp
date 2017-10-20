#include "Ball_InMatrix.h"

Ball_InMatrix::Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{
	setColor(b->getColor());
	//snapToGrid();

	neighbors = std::vector<Ball_InMatrix*>(6, NULL);
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

bool Ball_InMatrix::checkCollision(Ball * b)
{
	return false;
}

bool Ball_InMatrix::addNeighbor(Ball_InMatrix * b, int pos)
{
	if (pos < 6 && pos >= 0) {
		neighbors[pos] = b;
		return true;
	}
	return false;
}

vector<Ball_InMatrix*> Ball_InMatrix::getDisconnected()
{
	return vector<Ball_InMatrix*>();
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