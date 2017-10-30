#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_ANGLE 1.f							//In rads, or 1*180/pi degrees
#define PIXEL_SIZE glm::vec2(16.f,64.f)			//Pixel size in spritesheet and in game

void Aimer::init(const glm::vec2 & pos, glm::vec2 &minRenderCoords, ShaderProgram & shaderProgram, BallManager *bmng)
{
	//Aimer position
	_position = pos - glm::vec2(0.f,16.f);
	//Minimum screen
	_minRenderCoords = minRenderCoords;
	//Ball manager
	_bmng = bmng;
	//Aimer rotation
	_angle = 0;

	_spritesheet = new Texture();

	if (!_spritesheet->loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load spritesheet.png");
	}
	
	glm::vec2 sizeInSpriteSheet = glm::vec2(PIXEL_SIZE.x / _spritesheet->width(), PIXEL_SIZE.y / _spritesheet->height());
	//Quad size, Texture size, Texture position, Texture sheet, program
	_sprite = Sprite::createSprite(PIXEL_SIZE, PIXEL_SIZE, _spritesheet, &shaderProgram);
	//We don't need to update the aimer position at all so we pass minRendercoords to the sprite
	_sprite->setPosition(_position + _minRenderCoords);
	//Ball_Held from BallManager
	getNewHeldBall();
	
}

void Aimer::update(int deltaTime)
{
	//If there is no launched ball flying around
	if (!_bmng->ballUpdatesLeft()) {
		//And there is no ball held, we ask for a new ball
		if (_heldBall == nullptr) getNewHeldBall();
	}
	//If we have a held ball, update it (position, etc)
	if (_heldBall != nullptr) _heldBall->update(deltaTime, _angle);

	checkButtons(deltaTime);
}

void Aimer::render()
{
	//Render aimer sprite rotated
	_sprite->render(_angle, glm::vec2(0.5f, 37.75f / PIXEL_SIZE.y));
	//Render if we are holding a ball
	if (_heldBall != nullptr) _heldBall->render();
}

void Aimer::getNewHeldBall()
{
	//Held ball at <nextBall> position
	_heldBall = _bmng->getNextHeldBall();
	//Init heldBall values (angle, position, etc)
	_heldBall->initHeldPosition(_position, PIXEL_SIZE);
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

	//Shoot with UP or SPACE
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32)) {
		if (_heldBall != nullptr) {
			//Tell ballManager to shoot the ball
			_bmng->launchHeldBall(_heldBall, _angle);
			//Set heldBall to null until ballmanager allows us in Aimer::update
			_heldBall = nullptr;
		}
	}
}