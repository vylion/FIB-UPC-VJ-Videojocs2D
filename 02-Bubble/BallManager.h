#ifndef _BALLMANAGER_INCLUDE
#define _BALLMANAGER_INCLUDE

#include <vector>
#include <string>
using namespace std;

#include "Ball.h"
#include "BallMatrix.h"

class BallManager
{
public:
	static BallManager *createBallManager(const string &levelFile, glm::vec2 &mapSize, ShaderProgram &shaderProgram);

	BallManager::BallManager(const string & levelFile, glm::vec2 & mapSize, ShaderProgram & shaderProgram);

	//void init(const string &levelFile, glm::vec2 &mapSize, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render() const;

	bool ballUpdatesLeft();
	bool ballsLeft();
	
	Ball *getNextHeldBall();
	void launchHeldBall(Ball *heldBall, float angle);

private:
	int _spritePixelSize, _ballPixelSize;
	glm::ivec2 _matrixSize;
	glm::vec2 _ballTexSize, _spritesheetSize;
	glm::vec2 _ballSize;
	//Need these two for new balls
	Texture *_spritesheet; ShaderProgram _shaderProgram;
	//Flying ball, ball after current shot
	//Current ball is managed by aimer.cpp as it only needs to be displayed
	Ball *_launchedBall, *_nextBall;
	bool _thereIsLaunchedBall;
	//Static balls are managed by BallMatrix
	BallMatrix * _bmat;

	Ball *getNewBall();
	bool readLevel(const string & levelFile, glm::vec2 &mapSize);
	void setUpBalls(int *colorMatrix, int visibleMatrixHeight);
};
#endif