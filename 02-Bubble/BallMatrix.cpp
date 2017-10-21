#include "BallMatrix.h"

BallMatrix::BallMatrix( int * colorMatrix,
						glm::ivec2 &matrixDimensions,
						int visibleMatrixHeight,
						glm::vec2 _minBallCoords,
						const int &ballSize,
						const glm::vec2 &ballSizeInSpritesheet,
						Texture *spritesheet,
						ShaderProgram &shaderProgram)
{
	_matrixOffset = _minBallCoords;
	_visibleMatrixHeight = visibleMatrixHeight;
	_ballSize = ballSize;
	_ballSizeInSpritesheet = ballSizeInSpritesheet;
	_spritesheet = spritesheet;
	_shaderProgram = shaderProgram;

	int iterated = 0;
	int visibleOffset = matrixDimensions.y - visibleMatrixHeight;
	//_connectedMatrix = vector<vector<bool> >(matrixDimensions.x, vector<bool>(matrixDimensions.y, false));
	for (int i = 0; i < matrixDimensions.y; ++i) {
		vector<Ball_InMatrix*> ballRow;
		//If row is odd, consider 1 less ball to give hex pattern
		for (int j = 0; j < matrixDimensions.x - i%2; ++j) {
			Ball_InMatrix *b = ballFromColor(colorMatrix[iterated]);
			//Change to in-screen limits
			b->init(colorMatrix[iterated], glm::vec2(_matrixOffset.y*(j + 3), _matrixOffset.x + (i - visibleOffset)*ballSize ));
			b->setOddRow((i % 2 != 0));
			//To account for displacement
			ballRow.push_back(b);
			//_connectedMatrix[i][j] = true;
			iterated++;
		}
		_ballMatrix.push_back(ballRow);
	}
}

void BallMatrix::update(int &deltaTime)
{
	//Rows
	for (int i = 0; i < int(_ballMatrix.size()); ++i) {
		//Balls
		for (int j = 0; j < int(_ballMatrix[i].size()); ++j) {
			//_ballMatrix[i][j]->update(deltaTime);
		}
	}
}

void BallMatrix::render()
{
	//Rows
	for (int i = _ballMatrix.size() - 1; i >= int(_ballMatrix.size()) - _visibleMatrixHeight && i >= 0; --i) {
		//Balls
		for (int j = 0; j < int(_ballMatrix[i].size()); ++j) {
			_ballMatrix[i][j]->render();
		}
	}
}

bool BallMatrix::checkCollision(Ball * b)
{
	return false;

	Ball_InMatrix::posT pos = snapToGrid(b);
	list<Ball_InMatrix::posT> group = checkNeighbors(pos);

	while (group.size() > 0) {
		Ball_InMatrix::posT nextBall = group.front();
		group.pop_front();

		group.merge(_ballMatrix[nextBall.first][nextBall.second]->checkNeighbors());
	}
	
	/*
	glm::vec2 posB = snapToGrid(b);

	if		(_ballMatrix[posB.x - 1 + (int(posB.x) % 2)][posB.y - 1]) return true;		//TOP LEFT
	else if (_ballMatrix[posB.x  + (int(posB.x) % 2)][posB.y - 1]) return true;			//TOP RIGHT
	else if (_ballMatrix[posB.x - 1][posB.y]) return true;								//LEFT
	else if (_ballMatrix[posB.x + 1][posB.y]) return true;								//RIGHT
	else if (_ballMatrix[posB.x - 1 + (int(posB.x) % 2)][posB.y + 1]) return true;		//BOTTOM LEFT
	else if (_ballMatrix[posB.x + (int(posB.x) % 2)][posB.y + 1]) return true;			//BOTTOM RIGHT

	/*for (int j = 0; j < 3; ++j) {
		if (_ballMatrix[i][j]->checkCollision(b)) return true;
	}*/

	return false;
}

void BallMatrix::addBallToMat(Ball * b)
{
	//if falls
}

int BallMatrix::ballsLeft()
{
	return _ballMatrix.size();
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
	for (int i = 0; i < int(_ballMatrix.size()); ++i) {
		for (int j = 0; j < int(_ballMatrix[i].size()); ++j) {
			glm::vec2 currentPos = _ballMatrix[i][j]->getPosition();
			glm::vec2 nextPos = glm::vec2(currentPos.x, currentPos.y + _ballSize);
			_ballMatrix[i][j]->setPosition(nextPos);
		}
	}
}

Ball_InMatrix::posT BallMatrix::snapToGrid(Ball *b)
{
	glm::vec2 pos = b->getPosition();
	Ball_InMatrix::posT posInMatrix;

	pos -= glm::vec2(_matrixOffset.x, _matrixOffset.y);
	posInMatrix = Ball_InMatrix::posT(int(pos.x / _ballSize), int(pos.y / _ballSize));

	return posInMatrix;
}

list<Ball_InMatrix::posT> BallMatrix::checkNeighbors(const Ball_InMatrix::posT &b)
{
	return list<Ball_InMatrix::posT>();
}
