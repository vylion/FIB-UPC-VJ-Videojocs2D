#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define SHOOT_COOLDOWN 50						//Time between ball stop and next shot
#define MAX_ANGLE 1.f							//Angle will be 75 degrees -> radians
#define PIXEL_SIZE glm::vec2(16.f,64.f)			//Pixel size in spritesheet

void Aimer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, BallManager *bmng)
{

	_position = tileMapPos * 16;
	_size = PIXEL_SIZE;
	_canShoot = true;
	_angle = 0;

	_spritesheet = new Texture();

	if (!_spritesheet->loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load spritesheet.png");
	}
	//Quad size, Texture size, Texture position, Texture sheet, program
	glm::vec2 sizeInSpriteSheet = glm::vec2(PIXEL_SIZE.x / _spritesheet->width(), PIXEL_SIZE.y / _spritesheet->height());
	_sprite = Sprite::createSprite(PIXEL_SIZE, sizeInSpriteSheet, _spritesheet, &shaderProgram);
	_sprite->setPosition(tileMapPos*16);
	_bmng = bmng;
	//heldball from manager
	_heldBall = new Ball_Held(shaderProgram, _bmng->getNextHeldBall());
	_heldBall->init(_position, _size);
}

void Aimer::update(int deltaTime)
{
	//Control movement
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		_angle -= float(deltaTime)/400.f;
		_angle = std::max(_angle, -MAX_ANGLE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		_angle += float(deltaTime)/400.f;
		_angle = std::min(_angle, MAX_ANGLE);
	}

	//Shoot with UP or SPACE
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32) ) {
		if (_canShoot) { 
			_bmng->launchHeldBall(_heldBall, _angle);

			//heldball from manager
			_heldBall = _bmng->getNextHeldBall();
			_heldBall->init(_position, _size);
			_shootTime = SHOOT_COOLDOWN;
			_canShoot = false;
			
		}
	}
	_heldBall->update(deltaTime, _angle);
	_shootTime = std::max(0, _shootTime - deltaTime);
	_canShoot = (_shootTime == 0 && !_bmng->ballUpdatesLeft());

}

void Aimer::render()
{

	_sprite->render(_angle, glm::vec2(0.5f, 37.75f / PIXEL_SIZE.y));
	_heldBall->render();
}

void Aimer::ballToHeldball(Ball * b)
{
}
