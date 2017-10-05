#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define SHOOT_COOLDOWN 50						//Time between ball stop and next shot
#define MAX_ANGLE 0.75f							//Angle will be 75 degrees -> radians

void Aimer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, BallManager & bmng)
{
	
	_canShoot = true;
	_angle = 0;

	if (!_spritesheet.loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("A");
	}
	//Quad size, Texture size, Texture position, Texture sheet, program
	_sprite = Sprite::createSprite(glm::ivec2(16, 64), glm::vec2(16.f, 64.f), glm::vec2(32.f, 0.f), &_spritesheet, &shaderProgram);
	_sprite->setPosition(tileMapPos*16);
	_bmng = &bmng;

}

void Aimer::update(int deltaTime)
{
	//Control movement
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		_angle -= float(deltaTime)/200.f;
		_angle = std::max(_angle, -MAX_ANGLE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		_angle += float(deltaTime)/200.f;
		_angle = std::min(_angle, MAX_ANGLE);
	}

	//Shoot with UP or SPACE
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32) ) {
		if (_canShoot) {
			//_bmng->launchHeldBall(_heldBall, _angle);
			//_heldBall = _bmng->getNextHeldBall();
			_shootTime = SHOOT_COOLDOWN;
			_canShoot = false;
			
		}
	}
	//_heldBall->update(deltaTime, _angle);
	_shootTime = std::max(0, _shootTime - deltaTime);
	_canShoot = (_shootTime == 0 && !_bmng->ballUpdatesLeft());

}

void Aimer::render()
{
	_sprite->render(_angle);
	//_heldBall->render();
}