#include <iostream> //read from file
#include <stdlib.h> //rand
#include <time.h>	//rand init
#include <fstream>  //read from file
#include <sstream>	//read from file

#include "BallManager.h"
#include "Ball_Launched.h"


BallManager * BallManager::createBallManager(const string & levelFile, TileMap *tmap, ShaderProgram & shaderProgram)
{
	BallManager *bm = new BallManager(levelFile, tmap, shaderProgram);

	return bm;
}


BallManager::BallManager(const string & levelFile, TileMap *tmap, ShaderProgram & shaderProgram)
{
	_shaderProgram = shaderProgram;
	_tmap = tmap;

	srand(time(NULL));
	if (!readLevel(levelFile)) printf("BallManager: Failed to read levelFile");
	
}



//void BallManager::init(glm::ivec2 &minBallCoords)
void BallManager::init()
{
	//_minBallCoords = glm::ivec2(minBallCoords);
	_minBallCoords = _tmap->getMinRenderCoords() + _tmap->getBallOffset();
	_nextBall = getNewBall();
	_thereIsLaunchedBall = false;
}

void BallManager::update(int deltaTime)
{
	_bmat->update(deltaTime);
	if (_thereIsLaunchedBall) {

		_launchedBall->update(deltaTime);
		if (_bmat->checkCollision(_launchedBall))
			_bmat->addBallToMat(_launchedBall);
	}
}

void BallManager::render() const
{
	if (_thereIsLaunchedBall) _launchedBall->render();
	_bmat->render();
	_nextBall->render();
}

bool BallManager::ballUpdatesLeft()
{
	//if (_launchedBall != NULL) return _launchedBall->updatesLeft();
	//else return false;
	return false;
}

bool BallManager::ballsLeft()
{
	return (_bmat->ballsLeft() > 0);
}

Ball_Held * BallManager::getNextHeldBall()
{
	
	//Store next held ball for the return
	Ball_Held *ret = new Ball_Held(_shaderProgram, _nextBall);

	//Generate a new ball for display
	_nextBall = getNewBall();
	_nextBall->setPosition(glm::vec2(400.f, 400.f));
	return ret;
}

void BallManager::launchHeldBall(Ball_Held * heldBall, float angle)
{
	_launchedBall = new Ball_Launched(_shaderProgram, heldBall, heldBall->getAngle(), _minBallCoords, _matrixSpace);
	_thereIsLaunchedBall = true;
	//_launchedBall = new LaunchedBall(heldBall, angle);
	//_heldBall->launch(angle);
}

Ball * BallManager::getNewBall()
{
	int color = rand()%11;

	Ball *b = new Ball(_ballPixelSize, _ballTexSize, _spritesheet, _shaderProgram);
	b->init(color, glm::vec2(0.f, 0.f));

	return b;
}

bool BallManager::readLevel(const string & levelFile)
{
	//glm::vec2 mapSize = _tmap->getBallSpace();
	//_matrixSpace = glm::vec2(mapSize.x, mapSize.y);

	ifstream fin;
	string line, spritesheetFile;
	stringstream sstream;
	char ballColor;
	int visibleMatrixHeight;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "BALLMAP") != 0)
		return false;
	//Map size
	getline(fin, line);
	sstream.str(line);
	sstream >> _matrixTileSize.x >> _matrixTileSize.y >> visibleMatrixHeight;
	//Check if the balls fit in the hole :^)

	if (_matrixTileSize.x > _tmap->getBallSpace().x || visibleMatrixHeight > _tmap->getBallSpace().y) return false;
	//Tile and block size
	getline(fin, line);
	sstream.str(line);
	sstream >> _spritePixelSize >> _ballPixelSize;
	//_ballTexSize = glm::vec2(_spritePixelSize, _spritePixelSize);
	//Tilesheet name
	getline(fin, line);
	sstream.str(line);
	sstream >> spritesheetFile;
	_spritesheet = new Texture();
	_spritesheet->loadFromFile(spritesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	_spritesheet->setWrapS(GL_CLAMP_TO_EDGE);
	_spritesheet->setWrapT(GL_CLAMP_TO_EDGE);
	_spritesheet->setMinFilter(GL_NEAREST);
	_spritesheet->setMagFilter(GL_NEAREST);
	//Number of tiles in _spritesheet
	getline(fin, line);
	sstream.str(line);
	sstream >> _spritesheetSize.x >> _spritesheetSize.y;
	_ballTexSize = glm::vec2(1.f / _spritesheetSize.x, 1.f / _spritesheetSize.y);

	int *colorMatrix = new int[_matrixTileSize.x * _matrixTileSize.y];
	int iterated = 0;
	for (int j = 0; j<_matrixTileSize.y; j++)
	{
		//Account for odd rows having less balls (0 = even!)
		for (int i = 0; i<_matrixTileSize.x - j%2; i++)
		{
			fin.get(ballColor);
			
			//Parse hex values
			if (ballColor >= int('A') && ballColor <= int('F')) colorMatrix[iterated] = ballColor - int('A') + 10;
			else if (ballColor >= int('0') && ballColor <= int('9')) colorMatrix[iterated] = ballColor - int('0');
			else colorMatrix[iterated] = 0;

			iterated++;
		}
		fin.get(ballColor);
#ifndef _WIN32
		fin.get(ballColor);
#endif
	}
	fin.close();

	setUpBalls(colorMatrix, visibleMatrixHeight);

	return true;
}

void BallManager::setUpBalls(int *colorMatrix, int visibleMatrixHeight)
{
	_bmat = new BallMatrix(colorMatrix, _matrixTileSize, visibleMatrixHeight, _minBallCoords, _ballPixelSize, _ballTexSize, _spritesheet, _shaderProgram);
}
