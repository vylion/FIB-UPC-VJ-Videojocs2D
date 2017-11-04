#include "Ball_InMatrix.h"

Ball_InMatrix::Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{

	setColor(b->getColor());
	//snapToGrid();

	neighbors = std::vector<posT>();
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
	float distance = sqrt(pow(abs(getPosition().x - b->getPosition().x),2) + pow(abs(getPosition().y - b->getPosition().y),2));

	if (distance < getSize()) {
		if (b->getPosition().y - getPosition().y > getSize()*cos(30)) {
			//BOT
			if (b->getPosition().x - getPosition().x > 0) {
				//RIGHT
				return BOT_RIGHT;
			}
			else return BOT_LEFT;
		}
		else if (b->getPosition().y - getPosition().y < -getSize()*cos(30)) {
			//TOP
			if (b->getPosition().x - getPosition().x > 0) {
				//RIGHT
				return TOP_RIGHT;
			}
			else return TOP_LEFT;
		}
		else {
			//LATERAL
			if (b->getPosition().x - getPosition().x > 0) {
				//RIGHT
				return RIGHT;
			}
			else return LEFT;
		}
	}

	return OUTSIDE;
}

bool Ball_InMatrix::addNeighbor(Ball_InMatrix::posT b)
{
	neighbors.push_back(b);

	return neighbors.size() <= 6;
}

std::vector<Ball_InMatrix::posT> Ball_InMatrix::getNeighbors()
{
	return neighbors;
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