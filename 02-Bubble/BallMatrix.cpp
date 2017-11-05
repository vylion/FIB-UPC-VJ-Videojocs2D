#include "BallMatrix.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

	_connectedMatrix = std::vector< std::vector<bool> >(visibleOffset + levelHeight + 1, std::vector<bool>((int)matrixDimensions.x, false));

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
				if (validBall(neighbor)) {
					b->addNeighbor(neighbor);
					ballRow[j-1]->addNeighbor(posT(i, j));
				}

				//Check if there's a valid neighbor on the upper row, to the left. Add each other as neighbors
				neighbor = posT(i - 1, j - 1 + (i % 2));
				if (validBall(neighbor)) {
					b->addNeighbor(neighbor);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(posT(i, j));
				}

				//Check if there's a valid neighbor on the upper row, to the right. Add each other as neighbors
				neighbor = posT(i - 1, j + (i % 2));
				if (validBall(neighbor)) {
					b->addNeighbor(neighbor);
					_ballMatrix[neighbor.first][neighbor.second]->addNeighbor(posT(i, j));
				}
			}

			iterated++;
		}
		_ballMatrix.push_back(ballRow);
	}
	
	int limit = levelHeight - visibleMatrixHeight + _ballMatrix.size();
	for (int i = _ballMatrix.size(); i <= limit; ++i) {
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
	fallingBalls = vector<Ball_Falling*>();
	descendAnimLeft = 0;
	shakeAnim = false;
	updateFrontier();
	shakeCount = 0;
	shakeSide = 1;

	for (int i = 0; i < (int)_ballMatrix[0].size(); ++i) {
		_ballMatrix[0][i]->setTopRow(true);
	}

	srand((unsigned)time(0));
	float k = float((rand() % 360)*M_PI / 180);
	shake = glm::vec2(sin(k), cos(k));
}

BallMatrix::State BallMatrix::update(int &deltaTime)
{
	if (shakeAnim) {
		shakeCount += deltaTime;
		if (shakeCount > SHAKE_COUNT_MAX) {
			shakeCount = 0;
			shakeSide = -shakeSide;
			float k = float((rand() % 360)*M_PI/180);
			shake = glm::vec2(shakeSide*sin(k), shakeSide*0.6f*cos(k));
		}
	}

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
	for (int j = 0; j < (int)_ballMatrix[last].size(); ++j) {
		//_ballMatrix[i][j]->update(deltaTime);
		if (_connectedMatrix[last][j]) return LOST;
	}
	
	//First row has at least 1 ball -> game keeps running
	for (int j = 0; j < (int)_ballMatrix[0].size(); ++j) {
		//_ballMatrix[i][j]->update(deltaTime);

		if (_connectedMatrix[0][j]) {
			vector<Ball_Falling*>::iterator it = fallingBalls.begin();
			Ball_Falling* current;

			while (it != fallingBalls.end()) {
				current = *it;
				current->update(deltaTime);

				if (current->dead()) it = fallingBalls.erase(it);
				else ++it;
			}

			return RUNNING;
		}
	}

	return WON;
}

void BallMatrix::render()
{
	for (int i = 0; i < fallingBalls.size(); ++i) {
		fallingBalls[i]->render();
	}

	if (shakeAnim) {
		for (int i = _ballMatrix.size() - 1; i >= (int)_ballMatrix.size() - _visibleMatrixHeight && i >= 0; --i) {
			//Balls
			for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
				if (_connectedMatrix[i][j]) _ballMatrix[i][j]->render(shake);
			}
		}
	}

	else if (descendAnimLeft > 0) {
		float descendPercent = ((float)DESCEND_ANIM_TIME - (float)descendAnimLeft) / (float)DESCEND_ANIM_TIME;
		glm::vec2 descend = glm::vec2(0, descendPercent * _ballSize);

		int size = min(_ballSize, (int)(descendPercent * _ballSize));
		glm::vec2 pos = glm::vec2((float)_ballSize / 2.f) - glm::vec2(descendPercent * _ballSize / 2.f);
		pos.y += _ballSize; //So it appears one row below; the row it will snap to, in passRowToShown

		int newRow = (int)_ballMatrix.size() - _visibleMatrixHeight - 1;

		if (newRow >= 0) {
			for (int j = 0; j < (int)_ballMatrix[newRow].size(); ++j) {
				if (_connectedMatrix[newRow][j]) {
					_ballMatrix[newRow][j]->setSize(size);
					_ballMatrix[newRow][j]->render(pos);
				}
			}
		}

		for (int i = _ballMatrix.size() - 1; i >= newRow + 1 && i >= 0; --i) {
			//Balls
			for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
				if (_connectedMatrix[i][j]) _ballMatrix[i][j]->render(descend);
			}
		}
	}
	
	else {
		//Rows
		for (int i = _ballMatrix.size() - 1; i >= (int)_ballMatrix.size() - _visibleMatrixHeight && i >= 0; --i) {
			//Balls
			for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
				if (_connectedMatrix[i][j]) _ballMatrix[i][j]->render();
			}
		}
	}
}

bool BallMatrix::checkCollision(Ball * b)
{
	posT pos, ballPos;
	bool collision = false;;

	int visibleOffset = (int)_ballMatrix.size() - _visibleMatrixHeight;
	if (b->getPosition().y <= (float)_minBallCoords.y - visibleOffset*_ballSize + 0.25f) {
		//COLLISIONS WITH CEILING
		pos = posT(0, (b->getPosition().x - _minBallCoords.x) / (float)_ballSize);
		ballPos = snapToGrid(b, pos);

		collision = true;
	}
	else {
		//COLLISIONS WITH BALLS
		Ball_InMatrix::NeighborBalls collided = Ball_InMatrix::OUTSIDE;

		for (int i = 0; i < _collisionFrontier.size() && collided == Ball_InMatrix::OUTSIDE; ++i) {
			pos = _collisionFrontier[i];
			collided = _ballMatrix[pos.first][pos.second]->checkCollision(b);
		}

		if (collided != Ball_InMatrix::OUTSIDE) {
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

			if (validBall(ballPos)) ballPos = snapToGrid(b, pos);

			collision = true;
		}
	}
	
	if (collision) {
		//COMMON TO ALL COLLISIONS
		bool success = addBallToMat(ballPos, b->getColor());
		vector<posT> pop = vector<posT>();
		unsigned int mask = (unsigned int)pow(2, b->getColor());
		checkPopping(ballPos, mask, pop);

		if (pop.size() >= 3) {
			for (unsigned int i = 0; i < pop.size(); ++i) {
				pos = pop[i];
				success = success && popBall(pos); // Animation and control
			}

			pop = checkNotHanging();
			for (unsigned int i = 0; i < pop.size(); ++i) {
				pos = pop[i];
				success = success && popBall(pos); // Animation and control
			}
		}

		updateFrontier();

		if (shakeAnim) {
			descendAnimLeft = DESCEND_ANIM_TIME;
			shakeAnim = false;
		}
		else {
			bool descend = true;
			int row = max((int)_ballMatrix.size() - _visibleMatrixHeight, 0);
			for (int i = 0; i < _ballMatrix[row].size() && descend; ++i) {
				descend = descend && !(_connectedMatrix[row][i]);
			}
			if (descend) descendAnimLeft = DESCEND_ANIM_TIME;
		}

		return success;
	}

	return false;
}

//Adds a collided ball into the matrix
bool BallMatrix::addBallToMat(posT ballPos, unsigned int color)
{
	int visibleOffset = _ballMatrix.size() - _visibleMatrixHeight;
	//if (visibleOffset < 0) visibleOffset = 0;

	_ballMatrix[ballPos.first][ballPos.second]->init(color, _minBallCoords + float(_ballSize) * glm::vec2(ballPos.second, ballPos.first - visibleOffset), _minRenderCoords);
	_ballMatrix[ballPos.first][ballPos.second]->setOddRow(ballPos.first % 2 != 0);

	posT pos;
	
	//TOP LEFT
	pos = posT(ballPos.first - 1, ballPos.second - 1 + (ballPos.first % 2));
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
	}

	//TOP RIGHT
	pos = posT(ballPos.first - 1, ballPos.second + (ballPos.first % 2));
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
	}

	//LEFT
	pos = posT(ballPos.first, ballPos.second - 1);
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
	}

	//RIGHT
	pos = posT(ballPos.first, ballPos.second + 1);
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
	}

	//BOTTOM LEFT
	pos = posT(ballPos.first + 1, ballPos.second - 1 + (ballPos.first % 2));
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
	}

	//BOTTOM RIGHT
	pos = posT(ballPos.first + 1, ballPos.second + (ballPos.first % 2));
	if (validBall(pos)) {
		_ballMatrix[ballPos.first][ballPos.second]->addNeighbor(pos);
		_ballMatrix[pos.first][pos.second]->addNeighbor(ballPos);
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


	for (int i = 0; i < (int)_ballMatrix.size(); ++i) {
		//Balls
		for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
			if (_connectedMatrix[i][j]) res = res | (unsigned int)pow(2, _ballMatrix[i][j]->getColor());
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
	int newRow = (int)_ballMatrix.size() - _visibleMatrixHeight - 1;
	if (newRow >= 0) {
		for (int j = 0; j < (int)_ballMatrix[newRow].size(); ++j) {
			_ballMatrix[newRow][j]->setSize(_ballSize);
		}
	}

	for (int i = 0; i < (int)_ballMatrix.size(); ++i) {
		for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
			glm::vec2 currentPos = _ballMatrix[i][j]->getPosition();
			glm::vec2 nextPos = glm::vec2(currentPos.x, currentPos.y + _ballSize);
			_ballMatrix[i][j]->setPosition(nextPos);
		}
	}
	_ballMatrix.pop_back();
	_connectedMatrix.pop_back();
	//--_visibleMatrixHeight;
}

//Gives the closest matrix position to the ball given
Ball_InMatrix::posT BallMatrix::snapToGrid(Ball *b, posT pos)
{
	posT ballPos;
	double minDist = pow(_ballMatrix.size()*_ballSize, 2) + pow(_ballMatrix.size()*_ballSize, 2);
	glm::vec2 coord;
	vector<posT> space = checkSpaceAround(pos);

	for (int i = 0; i < space.size(); ++i) {
		coord = _ballMatrix[space[i].first][space[i].second]->getPosition();

		double dist = pow(coord.x - b->getPosition().x, 2) + pow(coord.y - b->getPosition().y, 2);

		if (dist < minDist) {
			minDist = dist;
			ballPos = space[i];
		}
	}

	return ballPos;
}

//Gives the possible neighbours around an assumed empty position
std::vector<Ball_InMatrix::posT> BallMatrix::checkBallsAround(const posT &b)
{
	std::vector<posT> group = std::vector<posT>();

	//TOP LEFT
	posT pos = posT(b.first - 1, b.second - 1 + (b.first % 2));
	if (validBall(pos)) group.push_back(pos);

	//TOP RIGHT
	pos = posT(b.first - 1, b.second + (b.first % 2));
	if(validBall(pos)) group.push_back(pos);

	//LEFT
	pos = posT(b.first, b.second - 1);
	if (validBall(pos)) group.push_back(pos);

	//MIDDLE
	//pos = posT(b.first, b.second);
	//if (validBall(pos)) group.push_back(pos);

	//RIGHT
	pos = posT(b.first, b.second + 1);
	if (validBall(pos)) group.push_back(pos);

	//BOTTOM LEFT
	pos = posT(b.first + 1, b.second - 1 + (b.first % 2));
	if (validBall(pos)) group.push_back(pos);

	//BOTTOM RIGHT
	pos = posT(b.first + 1, b.second + (b.first % 2));
	if (validBall(pos)) group.push_back(pos);

	return group;
}

std::vector<Ball_InMatrix::posT> BallMatrix::checkSpaceAround(const posT &b)
{
	std::vector<posT> group = std::vector<posT>();

	//TOP LEFT
	posT pos = posT(b.first - 1, b.second - 1 + (b.first % 2));
	if (validSpace(pos)) group.push_back(pos);

	//TOP RIGHT
	pos = posT(b.first - 1, b.second + (b.first % 2));
	if (validSpace(pos)) group.push_back(pos);

	//LEFT
	pos = posT(b.first, b.second - 1);
	if (validSpace(pos)) group.push_back(pos);

	//MIDDLE
	pos = posT(b.first, b.second);
	if (validSpace(pos)) group.push_back(pos);

	//RIGHT
	pos = posT(b.first, b.second + 1);
	if (validSpace(pos)) group.push_back(pos);

	//BOTTOM LEFT
	pos = posT(b.first + 1, b.second - 1 + (b.first % 2));
	if (validSpace(pos)) group.push_back(pos);

	//BOTTOM RIGHT
	pos = posT(b.first + 1, b.second + (b.first % 2));
	if (validSpace(pos)) group.push_back(pos);

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
			//Check if position has already been marked for poppoing
			if (std::find(pop.begin(), pop.end(), next) == pop.end()) {
				//It has not been popped
				pop.push_back(next);
				checkPopping(next, color, pop);
			}
		}
	}
}

std::vector<Ball_InMatrix::posT> BallMatrix::checkNotHanging()
{
	std::vector<posT> pop = std::vector<posT>();

	std::vector< std::vector<bool> > connected = std::vector< std::vector<bool> >(_connectedMatrix.size(), std::vector<bool>((int)_connectedMatrix[0].size(), false));

	int i;
	posT p;
	for (i = 0; i < (int)_connectedMatrix[0].size(); ++i) {
		connected[0][i] = _connectedMatrix[0][i];
	}

	for (i = 1; i < (int)_connectedMatrix.size(); ++i) {
		for (int j = 0; j < (int)_connectedMatrix[i].size(); ++j) {
			if (_connectedMatrix[i][j]) {
				//TOP LEFT
				p = posT(i - 1, j - 1 + (i % 2));
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//TOP RIGHT
				p = posT(i - 1, j + (i % 2));
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//LEFT
				p = posT(i, j - 1);
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//RIGHT
				p = posT(i, j + 1);
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//BOTTOM LEFT
				p = posT(i + 1, j - 1 + (i % 2));
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//BOTTOM RIGHT
				p = posT(i + 1, j + (i % 2));
				if (inMatrixRange(p) && connected[p.first][p.second]) {
					connected[i][j] = true;
					continue;
				}

				//IF NONE
				pop.push_back(posT(i, j));
			}
		}
	}
	_connectedMatrix = connected;

	return pop;
}

bool BallMatrix::popBall(posT & p)
{
	_connectedMatrix[p.first][p.second] = false;
	// Do animations
	std::vector<posT> neighbors = _ballMatrix[p.first][p.second]->getNeighbors();
	_ballMatrix[p.first][p.second]->resetNeighbors();

	for (int i = 0; i < (int)neighbors.size(); ++i) {
		posT pos = neighbors[i];
		_ballMatrix[pos.first][pos.second]->removeNeighbor(p);
	}

	float k = float((rand() % 360)*M_PI / 180);
	Ball_Falling * b = new Ball_Falling(_shaderProgram, _ballMatrix[p.first][p.second], k);
	fallingBalls.push_back(b);

	return !_connectedMatrix[p.first][p.second];
}

//Returns true if the position is inside the matrix dimensions and that position contains a valid ball
bool BallMatrix::inMatrixRange(const posT & pos)
{
	return (pos.first >= 0 && pos.first < (int)_connectedMatrix.size()) && (pos.second >= 0 && pos.second < (int)_connectedMatrix[pos.first].size() - pos.first % 2);
}

bool BallMatrix::validBall(const posT & pos)
{
	return inMatrixRange(pos) && _connectedMatrix[pos.first][pos.second];
}

bool BallMatrix::validSpace(const posT & pos)
{
	return inMatrixRange(pos) && !_connectedMatrix[pos.first][pos.second];
}

void BallMatrix::updateFrontier()
{
	_collisionFrontier = vector<posT>();

	for (int i = 0; i < (int)_ballMatrix.size(); ++i) {
		for (int j = 0; j < (int)_ballMatrix[i].size(); ++j) {
			if (_connectedMatrix[i][j]) {
				int maxNeighbors = 6;

				if (i == 0) maxNeighbors -= 2;
				if (j == 0 || j == _ballMatrix[i].size() - 1) {
					if (i % 2 == 0) maxNeighbors -= 2;
					maxNeighbors--;
				}

				if ((int)_ballMatrix[i][j]->getNeighbors().size() < maxNeighbors) {
					_collisionFrontier.push_back(posT(i, j));
				}
			}
		}
	}
}
