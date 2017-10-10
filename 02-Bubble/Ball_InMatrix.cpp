#include "Ball_InMatrix.h"

enum NeighborBalls {
	TOP_LEFT, TOP_RIGHT, RIGHT, BOT_RIGHT, BOT_LEFT, LEFT
};

Ball_InMatrix::Ball_InMatrix(ShaderProgram & shaderProgram, Ball * b)
	: Ball(b->getSize(), b->getSpritesheetSize(), b->getTexture(), shaderProgram)
{
	setColor(b->getColor());
	snapToGrid();
}

bool Ball_InMatrix::getOddRow()
{
	return _oddRow;
}

void Ball_InMatrix::setOddRow(const bool & oddRow)
{
	_oddRow = oddRow;
	if (_oddRow) {
		//Will add half the sprite of distance if odd
		glm::vec2 oddPos = glm::vec2(getPosition().x + getSize().x / 2 * _oddRow, getPosition().y);
		setPosition(oddPos);
	}
}

bool Ball_InMatrix::checkCollision(Ball * b)
{
	return false;
}

void Ball_InMatrix::snapToGrid() {
	glm::vec2 closestValidPos = getPosition();
	int sizex = int(getSize().x);
	int sizey = int(getSize().y);
	// pos/size to get the lower snap point
	// %size to know at what above point we are right now
	// /size/2 to know if we are on the first or second half
	// *size to snap to correct point
	closestValidPos.x = float(int(closestValidPos.x) / sizex + ((int(closestValidPos.x) % sizex) / (sizex / 2)) * sizex);
	closestValidPos.y = float(int(closestValidPos.y) / sizey + ((int(closestValidPos.y) % sizey) / (sizey / 2)) * sizey);
	setPosition(closestValidPos);
}