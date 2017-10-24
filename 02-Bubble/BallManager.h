#pragma once

#include <vector>
#include <string>
using namespace std;

#include "../TileMap.h"

#include "Ball.h"
#include "Ball_Held.h"
#include "Ball_Launched.h"
#include "BallMatrix.h"

class BallManager
{
public:
	static BallManager *createBallManager(TileMap *tmap, ShaderProgram &shaderProgram);

	BallManager::BallManager(TileMap *tmap, ShaderProgram & shaderProgram);
	
	//void init(glm::ivec2 &minRenderCoords);
	void init(const string &levelFile);
	void update(int deltaTime);
	void render() const;

	bool ballUpdatesLeft();
	bool ballsLeft();
	
	Ball_Held *getNextHeldBall();
	void launchHeldBall(Ball_Held *heldBall, float angle);

private:
	//Size in spritesheet (Pixels) and size in screen (Pixels)
	int _spritePixelSize, _ballPixelSize;
	//Starting coords for balls (tilemap), Space available for balls (tilemap), Space used for balls (read from file)
	glm::ivec2 _minBallCoords, _matrixSpace, _matrixTileSize;
	//Ball size in spritesheet (0..1) and total spritesheet size
	glm::vec2 _ballTexSize, _spritesheetSize;
	//Need these two for new balls
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;
	//Flying ball, ball after current shot
	//Current ball is managed by aimer.cpp as it only needs to be displayed
	Ball *_nextBall;
	Ball_Launched *_launchedBall;
	bool _thereIsLaunchedBall;
	//Static balls are managed by BallMatrix
	BallMatrix * _bmat;

	//Need this for tilemap collisions
	TileMap *_tmap;

	//Create ball
	Ball *getNewBall();
	//Open new level
	bool readLevel(const string & levelFile);
	//Prepare ball matrix
	void setUpBalls(int *colorMatrix, int visibleMatrixHeight);
};