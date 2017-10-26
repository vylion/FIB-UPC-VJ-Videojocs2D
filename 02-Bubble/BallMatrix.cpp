#include "BallMatrix.h"

BallMatrix::BallMatrix( int * colorMatrix,
						glm::ivec2 &matrixDimensions,
						int visibleMatrixHeight,
						int levelHeight,
						glm::vec2 minBallCoords,
						glm::vec2 minRenderCoords,
						const int &ballSize,
						const glm::vec2 &ballSizeInSpritesheet,
						Texture *spritesheet,
						ShaderProgram &shaderProgram)
{
	_minBallCoords = minBallCoords;
	_minRenderCoords = minRenderCoords;
	_ballSize = ballSize;
	_ballSizeInSpritesheet = ballSizeInSpritesheet;
	_spritesheet = spritesheet;
	_shaderProgram = shaderProgram;

	int iterated = 0;
	int visibleOffset = matrixDimensions.y - visibleMatrixHeight;
	if (visibleOffset < 0) visibleOffset = 0;
	_connectedMatrix = std::vector< std::vector<bool> >(int(visibleOffset + levelHeight + 1), std::vector<bool>(int(matrixDimensions.x), false));

	for (int i = 0; i < matrixDimensions.y; ++i) {
		std::vector<Ball_InMatrix*> ballRow;

		//If row is odd, consider 1 less ball to give hex pattern
		for (int j = 0; j < matrixDimensions.x - i%2; ++j) {
			Ball_InMatrix *b = ballFromColor(colorMatrix[iterated]);
			//Change to in-screen limits
			b->init(colorMatrix[iterated], _minBallCoords + (float)ballSize * (glm::vec2(j, i - visibleOffset)), _minRenderCoords);
			//To account for displacement
			b->setOddRow((i % 2 != 0));
			ballRow.push_back(b);

			//If it's a valid ball
			if (colorMatrix[iterated] >= 0 && colorMatrix[iterated] <= 15) {
				_connectedMatrix[i][j] = true;

				//Check if there's a valid neighbor on the left. Add each other as neighbors
				Ball_InMatrix::posT neighbor = Ball_InMatrix::posT(i, j - 1);
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::LEFT);
					_ballMatrix[i][j - 1]->addNeighbor(Ball_InMatrix::posT(i, j), Ball_InMatrix::RIGHT);
				}

				//Check if there's a valid neighbor on the upper row, to the left. Add each other as neighbors
				neighbor = Ball_InMatrix::posT(i - 1 + (i % 2), j - 1);
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::TOP_LEFT);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(Ball_InMatrix::posT(i, j), Ball_InMatrix::BOT_RIGHT);
				}

				//Check if there's a valid neighbor on the upper row, to the right. Add each other as neighbors
				neighbor = Ball_InMatrix::posT(i + (i % 2), j - 1);
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::TOP_RIGHT);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(Ball_InMatrix::posT(i, j), Ball_InMatrix::BOT_LEFT);
				}
			}

			iterated++;
		}
		_ballMatrix.push_back(ballRow);
	}
	
	for (int i = visibleMatrixHeight; i <= levelHeight; ++i) {
		std::vector<Ball_InMatrix*> ballRow;

		for (int j = 0; j < matrixDimensions.x - i % 2; ++j) {
			int color = 15;
			Ball_InMatrix *b = ballFromColor(color);
			b->init(color, _minBallCoords + (float)ballSize * (glm::vec2(j, i - visibleOffset)), _minRenderCoords);
			//To account for displacement
			b->setOddRow((i % 2 != 0));
			ballRow.push_back(b);
		}

		_ballMatrix.push_back(ballRow);
	}

	_visibleMatrixHeight = levelHeight + 1;
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
	Ball_InMatrix::posT ballPos = snapToGrid(b);
	std::vector<Ball_InMatrix::posT> nearby = checkBallsAround(ballPos);
	Ball_InMatrix::NeighborBalls collided = Ball_InMatrix::OUTSIDE;
	Ball_InMatrix::posT pos;

	for (int i = 0; i < nearby.size() && collided == Ball_InMatrix::OUTSIDE; ++i) {
		pos = nearby[i];
		collided = _ballMatrix[pos.first][pos.second]->checkCollision(b);
	}
	if (collided != Ball_InMatrix::OUTSIDE) {
		Ball_InMatrix * bm = new Ball_InMatrix(_shaderProgram, b);

		int visibleOffset = _ballMatrix.size() - _visibleMatrixHeight;
		bm->init(b->getColor(), _minBallCoords + float(_ballSize) * glm::vec2(ballPos.second, ballPos.first - visibleOffset), _minRenderCoords);
		_ballMatrix[pos.first][pos.second] = bm;
		return true;
	}

	return false;
}

//Adds a collided ball into the matrix
void BallMatrix::addBallToMat(Ball * b, Ball_InMatrix::NeighborBalls pos)
{
	
}

int BallMatrix::ballsLeft()
{
	return _ballMatrix.size();
}

Ball_InMatrix * BallMatrix::ballFromColor(int & color)
{
	//Create ball at 0,0 with the set color
	Ball *b = new Ball(_ballSize, _ballSizeInSpritesheet, _spritesheet, _shaderProgram);
	b->init(color, glm::vec2(0.f, 0.f), _minRenderCoords);
	//Create ball_inmatrix using the stablished ball
	Ball_InMatrix * bim = new Ball_InMatrix(_shaderProgram, b);
	return bim;
}

//Destroys last row of matrix, and moves down all balls for the height of a row
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

//Gives the closest matrix position to the ball given
Ball_InMatrix::posT BallMatrix::snapToGrid(Ball *b)
{
	glm::vec2 pos = b->getPosition();

	int i = (pos.y - _minBallCoords.y) / _ballSize;
	//i += -1 - _matrixOffset.y;
	if (int(_ballMatrix.size()) - _visibleMatrixHeight > 0) i += int(_ballMatrix.size()) - _visibleMatrixHeight;

	int j = (pos.x - _minBallCoords.x) / _ballSize;
	//j += -2 - _matrixOffset.x;
	Ball_InMatrix::posT posInMatrix(i, j);
	return posInMatrix;
}

//Gives the possible neighbours around an assumed empty position
std::vector<Ball_InMatrix::posT> BallMatrix::checkBallsAround(const Ball_InMatrix::posT &b)
{
	std::vector<Ball_InMatrix::posT> group = std::vector<Ball_InMatrix::posT>();

	//TOP LEFT
	Ball_InMatrix::posT pos = Ball_InMatrix::posT(b.first - 1 + (b.first % 2), b.second - 1);
	if (inMatrix(pos)) group.push_back(pos);

	//TOP RIGHT
	pos = Ball_InMatrix::posT(b.first + (b.first % 2), b.second - 1);
	if(inMatrix(pos)) group.push_back(pos);

	//LEFT
	pos = Ball_InMatrix::posT(b.first - 1, b.second);
	if (inMatrix(pos)) group.push_back(pos);

	//RIGHT
	pos = Ball_InMatrix::posT(b.first + 1, b.second);
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM LEFT
	pos = Ball_InMatrix::posT(b.first - 1 + (b.first % 2), b.second + 1);
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM RIGHT
	pos = Ball_InMatrix::posT(b.first + (b.first % 2), b.second + 1);
	if (inMatrix(pos)) group.push_back(pos);

	return group;
}

//DESTROY
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

//Returns true if the position is inside the matrix dimensions and that position contains a valid ball
bool BallMatrix::inMatrix(const Ball_InMatrix::posT & pos)
{
	return (pos.first >= 0 && pos.first < _ballMatrix.size()) && (pos.second >= 0 && pos.second < _ballMatrix[pos.first].size()) && _connectedMatrix[pos.first][pos.second];
}
