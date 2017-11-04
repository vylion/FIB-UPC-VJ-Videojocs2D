#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ANGLE 1.f							//In rads, or 1*180/pi degrees
#define AIMER_SIZE glm::vec2(16.f,64.f)			//Pixel size in spritesheet and in game

#define LOADING_TIME 400
#define SHOOTING_TIME 200

void Aimer::init(const glm::vec2 & pos, glm::vec2 &minRenderCoords, Texture *tex, ShaderProgram & shaderProgram, BallManager *bmng)
{
	//Aimer position
	_position = pos - glm::vec2(0.f,16.f);
	//Minimum screen
	_minRenderCoords = minRenderCoords;
	//Ball manager
	_bmng = bmng;
	//Aimer rotation
	_angle = 0;

	_state = state::READY;
	_animationTime = 0;

	_spritesheet = tex;
	
	glm::vec2 sizeInSpriteSheet = glm::vec2(AIMER_SIZE.x / _spritesheet->width(), AIMER_SIZE.y / _spritesheet->height());
	//Quad size, Texture size, Texture position, Texture sheet, program
	_sprite = Sprite::createSprite(AIMER_SIZE, AIMER_SIZE, _spritesheet, &shaderProgram);
	//We don't need to update the aimer position at all so we pass minRendercoords to the sprite
	_sprite->setPosition(_position + _minRenderCoords);
	//Ball_Held from BallManager
	getNewHeldBall();
	_heldBall->initHeldPosition(calculateLaunchedBallPosition(), _angle);
	
}

void Aimer::update(int deltaTime, int &bmngState)
{
	switch (_state) {
		//Everything is ready but we need launchedBall to collide or disappear
		case state::W8_BALL_MANAGER:
			switch (bmngState) {
				//Wait until ballManager waits for us
				case BallManager::state::W8_AIMER:
					_state = state::READY;
					break;
			}
		case state::READY:
			checkButtons(deltaTime);
			break;
		//Just launched a ball, get a new ball and start animation
		case state::LAUNCHED_BALL:
			getNewHeldBall();
			_state = state::LOADING;
			_animationTime = 0;
			checkButtons(deltaTime);
			break;
		case state::SHOOTING:
			shootUpdate(deltaTime);
			break;
		//Animate the new heldBall
		case state::LOADING:
			loadUpdate(deltaTime);
			checkButtons(deltaTime);
			break;
	}	
}

void Aimer::render()
{
	
	//Render aimer sprite rotated
	_sprite->render(_angle, glm::vec2(0.5f, 37.75f / AIMER_SIZE.y));
	//Render ball
	_heldBall->render();
	
}

void Aimer::getNewHeldBall()
{
	//Held ball at <nextBall> position
	_heldBall = _bmng->getNextHeldBall();
	//_heldBall->initHeldPosition(_position, AIMER_SIZE);
}

void Aimer::checkButtons(int deltaTime)
{
	//Control movement (rotation)
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		_angle -= float(deltaTime) / 400.f;
		_angle = std::max(_angle, -MAX_ANGLE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		_angle += float(deltaTime) / 400.f;
		_angle = std::min(_angle, MAX_ANGLE);
	}


	//If ball is in place and ballManager is waiting for us
	if (_state == state::READY){
		//Shoot with UP or SPACE
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32)) {
			//Set to just launched
			_state = state::SHOOTING;
			//Init position and angle
			_heldBall->initHeldPosition(calculateLaunchedBallPosition(), _angle);
			//Set initial size to 0 for spawn effect
			int size = 0;
			_heldBall->setSize(size);
			_animationTime = 0;
		}
	}
}

void Aimer::loadUpdate(int deltaTime)
{
	_animationTime += deltaTime;
	float ratio = (float)_animationTime / (float)LOADING_TIME;
	_heldBall->setPosition(_position * ratio);
	if (_animationTime >= LOADING_TIME) {
		_state = state::W8_BALL_MANAGER;
	}
}

void Aimer::shootUpdate(int deltaTime)
{
	_animationTime += deltaTime;
	//Hasn't finished loading
	if (_animationTime <= SHOOTING_TIME) {
		//Update ball
		_heldBall->updateShooting(_animationTime, SHOOTING_TIME);
	}
	//Finished loading
	else {
		//Restore initial size
		_heldBall->updateShooting(SHOOTING_TIME, SHOOTING_TIME);
		//Launch ball
		_bmng->launchHeldBall(_heldBall, _angle);
		_state = state::LAUNCHED_BALL;
	}
}

void Aimer::swapUpdate(int deltaTime)
{
}

glm::vec2 Aimer::calculateLaunchedBallPosition()
{
	//Angle rotated 1/4 of circumference clockwise to calculate position correctly
	float angle = _angle - float(M_PI / 2);
	//Initial position at the middle of aimer
	glm::vec2 shootingPosition = _position + glm::vec2(0.f, AIMER_SIZE.y / 2);
	//Add position relative to angle
	shootingPosition.x += AIMER_SIZE.x * cos(_angle);
	shootingPosition.y += AIMER_SIZE.y / 4 * sin(_angle);

	return shootingPosition;
}
