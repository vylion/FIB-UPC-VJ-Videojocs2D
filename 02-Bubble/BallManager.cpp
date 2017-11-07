#include <iostream> //read from file
#include <stdlib.h> //rand
#include <time.h>	//rand init
#include <fstream>  //read from file
#include <sstream>	//read from file

#include "BallManager.h"
#include "Ball_Launched.h"
#include "../Game.h"

#define LAUNCH_BALL_SOUND "media/audio/sounds/ball_launch.ogg"

BallManager * BallManager::createBallManager(TileMap *tmap, ShaderProgram & shaderProgram)
{
	BallManager *bm = new BallManager(tmap, shaderProgram);

	return bm;
}


BallManager::BallManager(TileMap *tmap, ShaderProgram & shaderProgram)
{
	_shaderProgram = shaderProgram;
	_tmap = tmap;

	srand((unsigned int)time(NULL));	
}



//void BallManager::init(glm::ivec2 &minBallCoords)
void BallManager::init(const string & levelFile)
{
	_thrownBalls = 0;
	//Minimum float pos at which we can render a ball (top left)
	_minBallCoords = /*(glm::vec2)_tmap->getMinRenderCoords() + */(glm::vec2)_tmap->getBallOffset() * (float)_tmap->getTileSize();
	if (!readLevel(levelFile)) printf("BallManager: Failed to read levelFile");

	_colorsInMatrix = _bmat->colorsLeftInMatrix();
	_state = state::W8_AIMER;
}

int BallManager::update(int deltaTime)
{
	//Update ball matrix and store state for switching later on
	int matState = _bmat->update(deltaTime);

	switch (_state) {
		//Just launched a ball, move to waiting for ball
		case state::LAUNCHED_BALL:
			SoundManager::instance().playSound(LAUNCH_BALL_SOUND);
			_state = state::W8_LAUNCHED_BALL;
			//No break because we want to update the ball
		//Waiting for launched ball
		case state::W8_LAUNCHED_BALL:
			//Update position etc
			_launchedBall->update(deltaTime);
			//Check for collisions

			if (_bmat->checkCollision(_launchedBall)) {
				_state = state::W8_MATRIX;
				//TODO
				if(_thrownBalls%5 == 0) _bmat->lowerRowsOnNextBall();
			}
			//Check if launched ball is off bounds

			else if (_launchedBall->getPosition().y <= -_ballPixelSize || _launchedBall->getPosition().y > SCREEN_HEIGHT)
				_state = state::W8_AIMER;
			break;
		//Waiting for feedback from matrix after a collision
		case state::W8_MATRIX:
			switch (matState) {
				//Free to move. We wait for aimer instructions
				case BallMatrix::State::RUNNING:
					_state = state::W8_AIMER;
					break;
				//We win, tell Scene to display win panel
				case BallMatrix::State::WON:
					_state = state::WON;
					break;
				//Same as won state
				case BallMatrix::State::LOST:
					_state = state::LOST;
					break;
			}
			break;
	}
	//_bmat->update(deltaTime);
	return _state;
}

void BallManager::render() const
{
	if (_state == state::W8_LAUNCHED_BALL || _state == state::LAUNCHED_BALL) _launchedBall->render();
	_bmat->render();
	//_nextBall->render();
}

Ball_Held * BallManager::getNextHeldBall()
{
	Ball *b = getNewBall();
	//Store next held ball for the return
	Ball_Held *ret = new Ball_Held(_shaderProgram, b);
	ret->init(b->getColor(), b->getPosition(), _tmap->getMinRenderCoords());
	
	_colorsInMatrix = _bmat->colorsLeftInMatrix();
	//Generate a new ball for display
	//_nextBall = getNewBall();

	return ret;
}

void BallManager::launchHeldBall(Ball_Held * heldBall, float angle)
{
	_launchedBall = new Ball_Launched(_shaderProgram, heldBall, heldBall->getAngle(), _tmap);
	_launchedBall->init(heldBall->getColor(), heldBall->getPosition(), _tmap->getMinRenderCoords());

	_thrownBalls++;

	_state = state::LAUNCHED_BALL;
}

int BallManager::getAccumulatedScoreVariation()
{
	return -50*_thrownBalls;
}

Ball* BallManager::getNewBall()
{
	int color;
	std::vector<int> colors;
	for (int i = _minColor; i < _availableColors + _minColor; ++i) {
		colors.push_back(i);
	}

	std::random_shuffle(colors.begin(), colors.end());
	bool found = false;

	for (int i = 0; i < colors.size() && !found; ++i) {
		color = colors[i];
		found = ((unsigned int)pow(2, color) & _colorsInMatrix);
	}

	Ball *b = new Ball(_ballPixelSize, glm::vec2((float)_spritePixelSize), _spritesheet, _shaderProgram);
	b->init(color, _minBallCoords, _tmap->getMinRenderCoords());

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

	if (_matrixTileSize.x-1 > _tmap->getBallSpace().x ||
		visibleMatrixHeight >= _tmap->getBallSpace().y)
		return false;
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
	//Available colors and minimum color
	getline(fin, line);
	sstream.str(line);
	sstream >> _availableColors >> _minColor;
	//Individual sprite (ball) size
	_ballTexSize = glm::vec2((float)_spritePixelSize) / _spritesheet->getSize();
	//_ballTexSize = glm::vec2((float)_spritePixelSize / _spritesheetSize.x, (float)_spritePixelSize / _spritesheetSize.y);

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
			//else colorMatrix[iterated] = 0;
			else colorMatrix[iterated] = ballColor - int('F') + 10;

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
	_bmat = new BallMatrix(colorMatrix, _matrixTileSize, visibleMatrixHeight, _tmap->getBallSpace().y, _minBallCoords, _tmap->getMinRenderCoords(), _ballPixelSize, glm::vec2((float)_spritePixelSize), _spritesheet, _shaderProgram);
}
