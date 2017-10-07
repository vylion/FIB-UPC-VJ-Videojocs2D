#include <iostream>
#include <fstream>
#include <sstream>
#include "BallManager.h"


BallManager::BallManager(const string & levelFile, glm::vec2 & mapSize, ShaderProgram & shaderProgram)
{
	_shaderProgram = shaderProgram;
	if (!readLevel(levelFile, mapSize)) printf("BallManager: Failed to read levelFile");
	_nextBall = getNewBall();
	_thereIsLaunchedBall = false;
}


BallManager * BallManager::createBallManager(const string & levelFile, glm::vec2 & mapSize, ShaderProgram & shaderProgram)
{
	BallManager *bm = new BallManager(levelFile, mapSize, shaderProgram);

	return bm;
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

Ball * BallManager::getNextHeldBall()
{
	
	//Store next held ball for the return
	Ball *ret = _nextBall;
	//Generate a new ball for display
	_nextBall = getNewBall();
	_nextBall->setPosition(glm::vec2(400.f, 400.f));
	return ret;
}

void BallManager::launchHeldBall(Ball * heldBall, float angle)
{
	_launchedBall = heldBall;
	//_launchedBall = new LaunchedBall(heldBall, angle);
	//_heldBall->launch(angle);
}

Ball * BallManager::getNewBall()
{
	int color = 1;
	Ball *b = new Ball(_ballTexSize, _spritesheet, _shaderProgram);
	b->init(color, glm::vec2(0.f, 0.f), false);
	return b;
}

bool BallManager::readLevel(const string & levelFile, glm::vec2 &mapSize)
{
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
	sstream >> _matrixSize.x >> _matrixSize.y >> visibleMatrixHeight;
	//Check if the balls fit in the hole :^)
	if (_matrixSize.x >= mapSize.x-1)
		return false;
	//Tile and block size
	getline(fin, line);
	sstream.str(line);
	sstream >> _spritePixelSize >> _ballPixelSize;
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
	sstream >> _ballSheetSize.x >> _ballSheetSize.y;
	_ballTexSize = glm::vec2(1.f / _ballSheetSize.x, 1.f / _ballSheetSize.y);

	int *colorMatrix = new int[_matrixSize.x * _matrixSize.y];
	for (int j = 0; j<_matrixSize.y; j++)
	{
		//Account for odd rows having less balls (0 = even!)
		for (int i = 0; i<_matrixSize.x - i%2; i++)
		{
			fin.get(ballColor);
			if (ballColor == ' ')
				colorMatrix[j*_matrixSize.x + i] = 0;
			else
				colorMatrix[j*_matrixSize.x + i] = ballColor - int('0');
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
	_bmat = new BallMatrix(colorMatrix, _matrixSize, visibleMatrixHeight, glm::vec2(_ballPixelSize, _ballPixelSize), _ballTexSize, _spritesheet, _shaderProgram);
}
