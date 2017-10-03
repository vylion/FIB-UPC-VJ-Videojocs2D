#include "Aimer.h"

#define SHOOT_COOLDOWN 50
#define MAX_ANGLE 75 //Should change to radianss

void Aimer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)//, BallGenerator & bgen)
{
	_canShoot = true;
	_angle = 0;

	if (!_spritesheet.loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("A");
	}
	//How do i even sprite
	//Quad size, Texture size, Texture position, Texture sheet, program
	_sprite = Sprite::createSprite(glm::ivec2(16, 64), glm::vec2(16.f, 64.f), glm::vec2(32.f, 0.f), &_spritesheet, &shaderProgram);
	_sprite->setPosition(glm::ivec2(0, 0));
	//_bgen = bgen;
}

void Aimer::update(int deltaTime)
{
}

void Aimer::render()
{
	_sprite->render();
	//_heldBall->render();
}
