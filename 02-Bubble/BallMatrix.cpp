#include "BallMatrix.h"

BallMatrix::BallMatrix( int * colorMatrix,
						glm::ivec2 &matrixDimensions,
						int visibleMatrixHeight,
						glm::vec2 _ballOffset,
						const int &ballSize,
						const glm::vec2 &ballSizeInSpritesheet,
						Texture *spritesheet,
						ShaderProgram &shaderProgram)
{
	_matrixOffset = _ballOffset;
	_visibleMatrixHeight = visibleMatrixHeight;
	_ballSize = ballSize;
	_ballSizeInSpritesheet = ballSizeInSpritesheet;
	_spritesheet = spritesheet;
	_shaderProgram = shaderProgram;

	int iterated = 0;
	int visibleOffset = matrixDimensions.y - visibleMatrixHeight;
	if (visibleOffset < 0) visibleOffset = 0;
	_connectedMatrix = std::vector< std::vector<bool> >(int(matrixDimensions.y), std::vector<bool>(int(matrixDimensions.x), false));
	for (int i = 0; i < matrixDimensions.y; ++i) {
		std::vector<Ball_InMatrix*> ballRow;
		//If row is odd, consider 1 less ball to give hex pattern
		for (int j = 0; j < matrixDimensions.x - i%2; ++j) {
			Ball_InMatrix *b = ballFromColor(colorMatrix[iterated]);
			//Change to in-screen limits
			b->init(colorMatrix[iterated], glm::vec2(ballSize*(j + 2 + _matrixOffset.x), ballSize*(i + 1 + _matrixOffset.y - visibleOffset) ));
			b->setOddRow((i % 2 != 0));
			//To account for displacement
			ballRow.push_back(b);
			_connectedMatrix[i][j] = true;
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
			if(_connectedMatrix[i][j]) _ballMatrix[i][j]->render();
		}
	}
}

bool BallMatrix::checkCollision(Ball * b)
{
	Ball_InMatrix::posT pos = snapToGrid(b);
	std::vector<Ball_InMatrix::posT> nearby = checkBallsAround(pos);
	bool collided = false;

	for (int i = 0; i < nearby.size() && !collided; ++i) {
		pos = nearby[i];

		collided = _ballMatrix[pos.first][pos.second]->checkCollision(b);
	}

	//if (collided) return _ballMatrix[999].size();

	return collided;
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

	int i = pos.y / _ballSize;
	i += -1 - _matrixOffset.y;
	if (int(_ballMatrix.size()) - _visibleMatrixHeight > 0) i += int(_ballMatrix.size()) - _visibleMatrixHeight;

	int j = pos.x / _ballSize;
	j += -2 - _matrixOffset.x;
	Ball_InMatrix::posT posInMatrix(i, j);
	return posInMatrix;
}

std::vector<Ball_InMatrix::posT> BallMatrix::checkBallsAround(const Ball_InMatrix::posT &b)
{
	std::vector<Ball_InMatrix::posT> group = std::vector<Ball_InMatrix::posT>();

	//TOP LEFT
	Ball_InMatrix::posT pos = Ball_InMatrix::posT(b.first - 1 + (int(b.first) % 2), b.second - 1);
	if (inMatrix(pos)) group.push_back(pos);

	//TOP RIGHT
	pos = Ball_InMatrix::posT(b.first + (int(b.first) % 2), b.second - 1);
	if(inMatrix(pos)) group.push_back(pos);

	//LEFT
	pos = Ball_InMatrix::posT(b.first - 1, b.second);
	if (inMatrix(pos)) group.push_back(pos);

	//RIGHT
	pos = Ball_InMatrix::posT(b.first + 1, b.second);
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM LEFT
	pos = Ball_InMatrix::posT(b.first - 1 + (int(b.first) % 2), b.second + 1);
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM RIGHT
	pos = Ball_InMatrix::posT(b.first + (int(b.first) % 2), b.second + 1);
	if (inMatrix(pos)) group.push_back(pos);

	return group;
}

list<Ball_InMatrix::posT> BallMatrix::popNeighbors(const Ball_InMatrix::posT & b)
{
	list<Ball_InMatrix::posT> group = list<Ball_InMatrix::posT>(); //checkNeighbors(b);

	while (group.size() > 0) {
		Ball_InMatrix::posT nextBall = group.front();
		group.pop_front();

		group.merge(_ballMatrix[nextBall.first][nextBall.second]->checkNeighbors());
	}

	return list<Ball_InMatrix::posT>();
}

bool BallMatrix::inMatrix(const Ball_InMatrix::posT & pos)
{
	return (pos.first >= 0 && pos.first < _ballMatrix.size()) && (pos.second >= 0 && pos.second < _ballMatrix[pos.first].size()) && _connectedMatrix[pos.first][pos.second];
}
