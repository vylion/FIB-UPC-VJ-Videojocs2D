#pragma once

#include <vector>
#include <string>
using namespace std;

#include "Ball.h"
#include "Ball_Held.h"
#include "Ball_Launched.h"
#include "BallMatrix.h"

class BallManager
{
public:
	static BallManager *createBallManager(const string &levelFile, glm::ivec2 &minCoords, glm::vec2 &mapSize, ShaderProgram &shaderProgram);

	BallManager::BallManager(const string & levelFile, glm::ivec2 &minCoords, glm::vec2 & mapSize, ShaderProgram & shaderProgram);

	void update(int deltaTime);
	void render() const;

	bool ballUpdatesLeft();
	bool ballsLeft();
	
	Ball_Held *getNextHeldBall();
	void launchHeldBall(Ball_Held *heldBall, float angle);

private:
	int _spritePixelSize, _ballPixelSize;
	glm::ivec2 _minCoords, _matrixSpace, _matrixSize;
	glm::vec2 _ballTexSize, _spritesheetSize;
	//Need these two for new balls
	Texture *_spritesheet; ShaderProgram _shaderProgram;
	//Flying ball, ball after current shot
	//Current ball is managed by aimer.cpp as it only needs to be displayed
	Ball *_nextBall;
	Ball_Launched *_launchedBall;
	bool _thereIsLaunchedBall;
	//Static balls are managed by BallMatrix
	BallMatrix * _bmat;

	Ball *getNewBall();
	bool readLevel(const string & levelFile, glm::vec2 &mapSize);
	void setUpBalls(int *colorMatrix, int visibleMatrixHeight);
};