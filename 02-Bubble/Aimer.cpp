#include "Aimer.h"
#include "../Game.h"
#include <algorithm>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define SHOOT_COOLDOWN 50.f						//Time between ball stop and next shot
#define MAX_ANGLE 75.f/(360*(2*float(M_PI)))	//Angle will be 75 degrees -> radians

void Aimer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	
	_canShoot = true;
	_angle = 0;

	if (!_spritesheet.loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("A");
	}
	//Quad size, Texture size, Texture position, Texture sheet, program
	_sprite = Sprite::createSprite(glm::ivec2(16, 64), glm::vec2(16.f, 64.f), glm::vec2(32.f, 0.f), &_spritesheet, &shaderProgram);
	_sprite->setPosition(tileMapPos*16);
	//_bmng = bmng;

}

void Aimer::update(int deltaTime)
{
	
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		_angle += deltaTime;
		_angle = std::max(_angle, MAX_ANGLE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		_angle -= deltaTime;
		_angle = std::max(_angle, -MAX_ANGLE);
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey(32) ) {
		if (_canShoot) {
			/*
			_bmng->launchBall(_heldBall, _angle);
			_heldBall = _bmng->getNextBall();
			_shootTime = SHOOT_COOLDOWN;
			_canShoot = false;
			*/
		}
	}
	_shootTime = std::max(0, _shootTime - deltaTime);


}

void Aimer::render()
{
	_sprite->render();
}