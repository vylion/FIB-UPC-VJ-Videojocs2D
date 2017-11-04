#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ANGLE 1.f							//In rads, or 1*180/pi degrees

#define TEXTURE_FILE "../media/images/aimer_sprites.png"

#define CANNON_SIZE glm::vec2(8.f, 8.f)
#define CANNON_SPRITESHEET_POSITION glm::vec2(0.f,0.f)

#define MAINBOX_SIZE glm::vec2(32.f,48.f)
#define MAINBOX_DISPLACEMENT glm::vec2(-12.f, 0.f)
#define MAINBOX_SPRITESHEET_POSITION glm::vec2(24.f, 0.f)

#define SWAPBOX_SIZE glm::vec2(24.f, 24.f)
#define SWAPBOX_DISPLACEMENT MAINBOX_DISPLACEMENT + glm::vec2(-48.f, 20.f)
#define SWAPBOX_SPRITESHEET_POSITION glm::vec2(0.f, 16.f)

#define NEXTBALL_COORDS glm::vec2(280.f, 448.f)

#define LOADING_TIME 750
#define SHOOTING_TIME 250

void Aimer::init(const glm::vec2 &cannonPos, glm::vec2 &minRenderCoords, ShaderProgram & shaderProgram, BallManager *bmng)
{
	//Aimer position
	_cannonPosition = cannonPos + glm::vec2((16.f - CANNON_SIZE.x)/2, 24.f - CANNON_SIZE.y);
	//Minimum screen
	_minRenderCoords = minRenderCoords;
	//Ball manager
	_bmng = bmng;
	//Aimer rotation
	_angle = 0;

	_state = state::READY;
	_animationTime = 0;
	_animationRatio = 0.f;

	
	_spritesheet = new Texture();
	if (!_spritesheet->loadFromFile(TEXTURE_FILE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Couldnt load aimer sprites");
	
	//Quad size, Texture size, Texture position, Texture sheet, program
	_cannon = Sprite::createSprite(CANNON_SIZE, CANNON_SIZE, _spritesheet, &shaderProgram);
	//We don't need to update the aimer position at all so we pass minRendercoords to the sprite
	_cannon->setPosition(_cannonPosition + _minRenderCoords);
	_cannon->setNumberAnimations(6);
	for (int i = 0; i < 6; i++) {
		_cannon->addKeyframe(i, glm::vec2((int)CANNON_SIZE.x * i % (6/2), (int)CANNON_SIZE.y / (6/2)) / _spritesheet->getSize());
		_cannon->setAnimationSpeed(i, SHOOTING_TIME / 6);
	}


	_mainBox = Sprite::createSprite(MAINBOX_SIZE, MAINBOX_SIZE, _spritesheet, &shaderProgram);
	_mainBox->setPosition(_cannonPosition + MAINBOX_DISPLACEMENT + _minRenderCoords);
	_mainBox->setTexturePosition(MAINBOX_SPRITESHEET_POSITION / _spritesheet->getSize());

	_swapBox = Sprite::createSprite(SWAPBOX_SIZE, SWAPBOX_SIZE, _spritesheet, &shaderProgram);
	_swapBox->setPosition(_cannonPosition + SWAPBOX_DISPLACEMENT + _minRenderCoords);
	_swapBox->setTexturePosition(SWAPBOX_SPRITESHEET_POSITION / _spritesheet->getSize());

	//Ball_Held from BallManager
	_nextBall = _bmng->getNextHeldBall();
	getNewHeldBall();
	_heldBall->initHeldPosition(_cannonPosition + MAINBOX_DISPLACEMENT + glm::vec2(8.f, 24.f), _angle);
	_swappedBall = nullptr;
	
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
			checkButtons(deltaTime);
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
	_cannon->render(_angle, glm::vec2(0.5f, 1.f));
	//Render balls
	if (_nextBall != nullptr) {
		_nextBall->render();
	}
	_heldBall->render(_animationRatio*(float)M_PI*4.f);
	if (_swappedBall != nullptr) _swappedBall->render();

	
	_mainBox->render();
	_swapBox->render();
	
}

void Aimer::getNewHeldBall()
{
	//Held ball at <nextBall> position
	_heldBall = _nextBall;
	_nextBall = _bmng->getNextHeldBall();
	_nextBall->initHeldPosition(NEXTBALL_COORDS, _angle);
}

void Aimer::checkButtons(int deltaTime)
{
	//Control movement (rotation)
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		_angle -= float(deltaTime) / 500.f;
		_angle = std::max(_angle, -MAX_ANGLE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		_angle += float(deltaTime) / 500.f;
		_angle = std::min(_angle, MAX_ANGLE);
	}


	//If ball is in place and ballManager is waiting for us
	if (_state == state::READY){
		//Shoot with UP or SPACE
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32)) {
			//Set to just launched
			_state = state::SHOOTING;
			//_nextBall = _bmng->getNextHeldBall();
			//Init position and angle
			//_nextBall->initHeldPosition(NEXTBALL_COORDS, _angle);
			_heldBall->initHeldPosition(calculateLaunchedBallPosition(), _angle);
			//Set initial size to 0 for spawn effect
			int size = 0;
			_heldBall->setSize(size);
			//_nextBall->setSize(size);
			_animationTime = 0;
			//_cannon->changeAnimation(0);
			//_cannon->setTexturePosition((CANNON_SPRITESHEET_POSITION + glm::vec2(0,CANNON_SIZE.y)) / _spritesheet->getSize());
		}
	}
}

void Aimer::loadUpdate(int deltaTime)
{
	_animationTime += deltaTime;
	_nextBall->updateShooting(min(max(0, _animationTime - 500), SHOOTING_TIME), SHOOTING_TIME);
	_animationRatio = (float)_animationTime / (float)LOADING_TIME;
	_heldBall->setPosition(NEXTBALL_COORDS * (1 - _animationRatio) + (_cannonPosition + MAINBOX_DISPLACEMENT + glm::vec2(8.f, 24.f)) * _animationRatio);
	if (_animationTime >= LOADING_TIME) {
		_state = state::W8_BALL_MANAGER;
	}
}

void Aimer::shootUpdate(int deltaTime)
{
	_animationTime += deltaTime;
	_cannon->update(deltaTime);
	//Hasn't finished loading
	if (_animationTime <= SHOOTING_TIME) {
		//Update ball
		_heldBall->updateShooting(_animationTime, SHOOTING_TIME);
		//_nextBall->updateShooting(_animationTime, SHOOTING_TIME);
	}
	//Finished loading
	else {
		//Restore initial size
		_heldBall->updateShooting(SHOOTING_TIME, SHOOTING_TIME);
		//_nextBall->updateShooting(SHOOTING_TIME, SHOOTING_TIME);
		//Launch ball
		_bmng->launchHeldBall(_heldBall, _angle);
		//_cannon->changeAnimation(0);
		_state = state::LAUNCHED_BALL;
	}
}

void Aimer::swapUpdate(int deltaTime)
{
}

glm::vec2 Aimer::calculateLaunchedBallPosition()
{
	//Start on cannon beginning
	glm::vec2 shootingPosition = _cannonPosition;
	//We want the ball to be horizontally centered with the cannon so we will subtract
	//half the ball size from the middle the cannon position (pos + size/2)
	shootingPosition.x += CANNON_SIZE.x/2.f;
	shootingPosition.x -= (float)_heldBall->getSize()/2.f;
	//Subtract 12.f vertically so it is right above the cannon at all times regardless
	//of current angle
	shootingPosition.y -= (float)_heldBall->getSize()/4.f;

	//Angle rotated 1/4 of circumference clockwise to calculate position correctly
	float angle = _angle - float(M_PI / 2);
	shootingPosition.x += CANNON_SIZE.x  * 2.f * cos(angle);
	shootingPosition.y += CANNON_SIZE.y  * 1.5f * sin(angle);

	return shootingPosition;
}