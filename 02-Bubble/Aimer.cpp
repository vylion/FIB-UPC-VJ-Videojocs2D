#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ANGLE 1.f							//In rads, or 1*180/pi degrees
#define AIMER_SIZE glm::vec2(16.f,64.f)			//Pixel size in spritesheet and in game

#define ANIMATION_TIME 20000

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
	_heldBall->initHeldPosition(_position, AIMER_SIZE);
	
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
			break;
		//Just launched a ball, get a new ball and start animation
		case state::LAUNCHED_BALL:
			getNewHeldBall();
			_state = state::ANIMATING;
			_animationTime = 0;
			break;
		//Animate the new heldBall
		case state::ANIMATING:
			animationUpdate(deltaTime);
			break;
	}
	_heldBall->update(deltaTime, _angle);
	/*
	if (!_bmng->ballUpdatesLeft()) {
		//And there is no ball held, we ask for a new ball
		if (_heldBall == nullptr) getNewHeldBall();
	}
	//If we have a held ball, update it (position, etc)
	if (_heldBall != nullptr) 
	*/

	checkButtons(deltaTime);
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
	_heldBall->initHeldPosition(_position, AIMER_SIZE);
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
			//Tell ballManager to shoot the ball
			_bmng->launchHeldBall(_heldBall, _angle);
			//Set to just launched
			_state = state::LAUNCHED_BALL;
		}
	}
}

void Aimer::animationUpdate(int deltaTime)
{
	_animationTime += deltaTime;
	_heldBall->setPosition((_heldBall->getPosition() + _position) /2.f);
	if (_animationTime >= ANIMATION_TIME) _state = state::W8_BALL_MANAGER;
}
