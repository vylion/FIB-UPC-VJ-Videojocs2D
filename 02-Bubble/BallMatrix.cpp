#include "BallMatrix.h"

BallMatrix::BallMatrix( int * colorMatrix,
						glm::ivec2 &matrixDimensions,
						int visibleMatrixHeight,
						const glm::vec2 &ballSize,
						const glm::vec2 &ballSizeInSpritesheet,
						Texture *spritesheet,
						ShaderProgram &shaderProgram)
{
	_visibleMatrixHeight = visibleMatrixHeight;
	_ballSize = ballSize;
	_ballSizeInSpritesheet = ballSizeInSpritesheet;
	_spritesheet = spritesheet;
	_shaderProgram = shaderProgram;

	int iterated = 0;
	for (int i = 0; i < matrixDimensions.y; ++i) {
		vector<Ball_InMatrix*> ballRow;
		//If row is odd, consider 1 less ball to give hex pattern
		for (int j = 0; j < matrixDimensions.x - i%2; ++j) {
			Ball_InMatrix *b = ballFromColor(colorMatrix[iterated]);
			//Change to in-screen limits
			b->init(colorMatrix[iterated], glm::vec2(16.f*(j + 3), 32.f));
			b->setOddRow((i % 2 != 0));
			//To account for displacement
			ballRow.push_back(b);
			iterated++;
		}
		_hiddenBallMatrix.push_back(ballRow);
	}

	for (int i = 0; i < visibleMatrixHeight; ++i) {
		passRowToShown();
	}

}

void BallMatrix::update(int &deltaTime)
{
	//Rows
	for (int i = 0; i < int(_shownBallMatrix.size()); ++i) {
		//Balls
		for (int j = 0; j < int(_shownBallMatrix[i].size()); ++j) {
			//_shownBallMatrix[i][j]->update(deltaTime);
		}
	}
}

void BallMatrix::render()
{
	//Rows
	for (int i = 0; i < int(_shownBallMatrix.size()); ++i) {
		//Balls
		for (int j = 0; j < int(_shownBallMatrix[i].size()); ++j) {
			_shownBallMatrix[i][j]->render();
		}
	}
}

bool BallMatrix::checkCollision(Ball * b)
{
	//Rows
	for (int i = 0; i < int(_shownBallMatrix.size()); ++i) {
		//Balls
		for (int j = 0; j < int(_shownBallMatrix[i].size()); ++j) {
			if (_shownBallMatrix[i][j]->checkCollision(b)) return true;
		}
	}
	return false;
}

void BallMatrix::addBallToMat(Ball * b)
{
	//if falls
}

int BallMatrix::ballsLeft()
{
	return _hiddenBallMatrix.size() + _shownBallMatrix.size();
}

Ball_InMatrix * BallMatrix::ballFromColor(int & color)
{
	//Create ball at 0,0 with the set color
	Ball *b = new Ball(_ballSize, _ballSizeInSpritesheet, _spritesheet, _shaderProgram);
	b->init(color, glm::vec2(0.f, 0.f));
	//Create ball_inmatrix using the stablished ball
	Ball_InMatrix * bim = new Ball_InMatrix(_shaderProgram, b);
	return bim;
}

void BallMatrix::passRowToShown()
{
	for (int i = 0; i < int(_shownBallMatrix.size()); ++i) {
		for (int j = 0; j < int(_shownBallMatrix[i].size()); ++j) {
			glm::vec2 currentPos = _shownBallMatrix[i][j]->getPosition();
			glm::vec2 nextPos = glm::vec2(currentPos.x, currentPos.y + _ballSize.y);
			_shownBallMatrix[i][j]->setPosition(nextPos);
		}
	}
	_shownBallMatrix.push_back(_hiddenBallMatrix[_hiddenBallMatrix.size()-1]);
	_hiddenBallMatrix.erase(_hiddenBallMatrix.end()-1);
}
