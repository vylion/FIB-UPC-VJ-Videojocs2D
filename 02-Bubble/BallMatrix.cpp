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
	//if (visibleOffset < 0) visibleOffset = 0;

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
				posT neighbor = posT(i, j - 1);
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::LEFT);
					ballRow[j-1]->addNeighbor(posT(i, j), Ball_InMatrix::RIGHT);
				}

				//Check if there's a valid neighbor on the upper row, to the left. Add each other as neighbors
				neighbor = posT(i - 1, j - 1 + (i % 2));
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::TOP_LEFT);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(posT(i, j), Ball_InMatrix::BOT_RIGHT);
				}

				//Check if there's a valid neighbor on the upper row, to the right. Add each other as neighbors
				neighbor = posT(i - 1, j + (i % 2));
				if (inMatrix(neighbor)) {
					b->addNeighbor(neighbor, Ball_InMatrix::TOP_RIGHT);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(posT(i, j), Ball_InMatrix::BOT_LEFT);
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
	descendAnimLeft = 0;
	shakeAnim = false;
}

BallMatrix::State BallMatrix::update(int &deltaTime)
{
	if (descendAnimLeft > 0) {
		descendAnimLeft -= deltaTime;
		if (descendAnimLeft < 0) {
			descendAnimLeft = 0;
			passRowToShown();
		}
		return UPDATING;
	}

	//Last row has at least 1 ball -> lose
	int last = _ballMatrix.size() - 1;
	for (int j = 0; j < int(_ballMatrix[last].size()); ++j) {
		//_ballMatrix[i][j]->update(deltaTime);
		if (_connectedMatrix[last][j]) return LOST;
	}
	
	//First row has at least 1 ball -> game keeps running
	for (int j = 0; j < int(_ballMatrix[0].size()); ++j) {
		//_ballMatrix[i][j]->update(deltaTime);
		if (_connectedMatrix[0][j]) return RUNNING;
	}

	return WON;
}

void BallMatrix::render()
{
	//if (shakeAnim)
	//...
	//else

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
	posT ballPos = snapToGrid(b);
	std::vector<posT> nearby = checkBallsAround(ballPos);
	Ball_InMatrix::NeighborBalls collided = Ball_InMatrix::OUTSIDE;
	posT pos;

	for (unsigned int i = 0; i < nearby.size() && collided == Ball_InMatrix::OUTSIDE; ++i) {
		pos = nearby[i];
		collided = _ballMatrix[pos.first][pos.second]->checkCollision(b);
	}
	if (collided != Ball_InMatrix::OUTSIDE) {
		if (shakeAnim) {
			descendAnimLeft = DESCEND_ANIM_TIME;
			shakeAnim = false;
		}
		
		ballPos = posT(pos);
		if (collided == Ball_InMatrix::BOT_LEFT || collided == Ball_InMatrix::TOP_LEFT) {
			ballPos.second--;
		}
		if (collided == Ball_InMatrix::BOT_RIGHT || collided == Ball_InMatrix::BOT_LEFT) {
			ballPos.first++;
			ballPos.second += (pos.first % 2);
		}
		else if (collided == Ball_InMatrix::TOP_RIGHT || collided == Ball_InMatrix::TOP_LEFT) {
			ballPos.first--;
			ballPos.second += (pos.first % 2);
		}
		else {
			if (collided == Ball_InMatrix::LEFT) ballPos.second--;
			else ballPos.second++;
		}

		bool success = addBallToMat(ballPos, collided, b->getColor());
		vector<posT> pop = vector<posT>();
		unsigned int mask = (unsigned int)pow(2, b->getColor());
		checkPopping(ballPos, mask, pop);

		if (pop.size() >= 3) {
			for (unsigned int i = 0; i < pop.size(); ++i) {
				pos = pop[i];
				success = success && popBall(pos); // Animation and control
			}
		}

		return success;
	}

	return false;
}

//Adds a collided ball into the matrix
bool BallMatrix::addBallToMat(posT ballPos, Ball_InMatrix::NeighborBalls collided, unsigned int color)
{
	int visibleOffset = _ballMatrix.size() - _visibleMatrixHeight;
	//if (visibleOffset < 0) visibleOffset = 0;

	_ballMatrix[ballPos.first][ballPos.second]->init(color, _minBallCoords + float(_ballSize) * glm::vec2(ballPos.second, ballPos.first - visibleOffset), _minRenderCoords);
	_ballMatrix[ballPos.first][ballPos.second]->setOddRow(ballPos.first % 2 != 0);

	posT pos;
	
	//TOP LEFT
	pos = posT(ballPos.first - 1, ballPos.second - 1 + (ballPos.first % 2));
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::TOP_LEFT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::BOT_RIGHT);
	}

	//TOP RIGHT
	pos = posT(ballPos.first - 1, ballPos.second + (ballPos.first % 2));
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::TOP_RIGHT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::BOT_LEFT);
	}

	//LEFT
	pos = posT(ballPos.first, ballPos.second - 1);
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::LEFT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::RIGHT);
	}

	//RIGHT
	pos = posT(ballPos.first, ballPos.second + 1);
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::RIGHT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::LEFT);
	}

	//BOTTOM LEFT
	pos = posT(ballPos.first + 1, ballPos.second - 1 + (ballPos.first % 2));
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::BOT_LEFT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::TOP_RIGHT);
	}

	//BOTTOM RIGHT
	pos = posT(ballPos.first + 1, ballPos.second + (ballPos.first % 2));
	if (inMatrix(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos, Ball_InMatrix::BOT_RIGHT);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos, Ball_InMatrix::TOP_LEFT);
	}

	_connectedMatrix[ballPos.first][ballPos.second] = true;

	return _connectedMatrix[ballPos.first][ballPos.second];
}

int BallMatrix::ballsLeft()
{
	return _ballMatrix.size();
}

unsigned int BallMatrix::colorsLeftInMatrix()
{
	unsigned int res = 0;


	for (int i = 0; i < int(_ballMatrix.size()); ++i) {
		//Balls
		for (int j = 0; j < int(_ballMatrix[i].size()); ++j) {
			if (_connectedMatrix[i][j]) res = res | unsigned int(pow(2, _ballMatrix[i][j]->getColor()));
		}
	}

	return res;
}

void BallMatrix::lowerRowsOnNextBall()
{
	shakeAnim = true;
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

	//--_visibleMatrixHeight;
}

//Gives the closest matrix position to the ball given
Ball_InMatrix::posT BallMatrix::snapToGrid(Ball *b)
{
	glm::vec2 pos = b->getPosition();

	int i = (int)(pos.y - _minBallCoords.y) / _ballSize;
	//i += -1 - _matrixOffset.y;
	if (int(_ballMatrix.size()) - _visibleMatrixHeight > 0) i += int(_ballMatrix.size()) - _visibleMatrixHeight;

	int j;
	if (i % 2 == 0) {
		j = (int)(pos.x - _minBallCoords.x) / _ballSize;
	}
	else {
		j = (int)(pos.x - _minBallCoords.x) / _ballSize;
	}
	//j += -2 - _matrixOffset.x;
	
	int visibleOffset = _ballMatrix.size() - _visibleMatrixHeight;
	//if (visibleOffset < 0) visibleOffset = 0;
	posT posInMatrix(i + visibleOffset - (int)_minRenderCoords.y/_ballSize, j);
	return posInMatrix;
}

//Gives the possible neighbours around an assumed empty position
std::vector<Ball_InMatrix::posT> BallMatrix::checkBallsAround(const posT &b)
{
	std::vector<posT> group = std::vector<posT>();

	//TOP LEFT
	posT pos = posT(b.first - 1, b.second - 1 + (b.first % 2));
	if (inMatrix(pos)) group.push_back(pos);

	//TOP RIGHT
	pos = posT(b.first - 1, b.second + (b.first % 2));
	if(inMatrix(pos)) group.push_back(pos);

	//LEFT
	pos = posT(b.first, b.second - 1);
	if (inMatrix(pos)) group.push_back(pos);

	//MIDDLE
	pos = posT(b.first, b.second);
	if (inMatrix(pos)) group.push_back(pos);

	//RIGHT
	pos = posT(b.first, b.second + 1);
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM LEFT
	pos = posT(b.first + 1, b.second - 1 + (b.first % 2));
	if (inMatrix(pos)) group.push_back(pos);

	//BOTTOM RIGHT
	pos = posT(b.first + 1, b.second + (b.first % 2));
	if (inMatrix(pos)) group.push_back(pos);

	return group;
}

//DESTROY
void BallMatrix::checkPopping(const posT & b, const unsigned int & mask, std::vector<posT>& pop)
{
	vector<posT> group = _ballMatrix[b.first][b.second]->getNeighbors();
	
	for (unsigned int i = 0; i < group.size(); ++i) {
		posT next = group[i];
		unsigned int color = (unsigned int)pow(2, _ballMatrix[next.first][next.second]->getColor());

		if ((color & mask) != 0) {
			if (std::find(pop.begin(), pop.end(), next) == pop.end()) {
				//It has not been popped
				pop.push_back(next);
				checkPopping(next, color, pop);
			}
		}
	}
}

bool BallMatrix::popBall(posT & p)
{
	_connectedMatrix[p.first][p.second] = false;
	// Do animations

	return !_connectedMatrix[p.first][p.second];
}

//Returns true if the position is inside the matrix dimensions and that position contains a valid ball
bool BallMatrix::inMatrix(const posT & pos)
{
	return (pos.first >= 0 && pos.first < (int)_connectedMatrix.size()) && (pos.second >= 0 && pos.second < (int)_connectedMatrix[pos.first].size()) && _connectedMatrix[pos.first][pos.second];
}
