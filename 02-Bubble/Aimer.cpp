#include "Aimer.h"

#define SHOOT_COOLDOWN 50
#define MAX_ANGLE 75 //Should change to radianss

void Aimer::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	_canShoot = true;
	_angle = 0;
	_spritesheet.loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//How do i even sprite
	_sprite = Sprite::createSprite(glm::ivec2(32, 128), glm::vec2(0.25, 0.25), &_spritesheet, &shaderProgram);
}

void Aimer::update(int deltaTime)
{
}

void Aimer::render()
{
}
