#pragma once
#include "../Sprite.h"
//Soon(TM)
//#include "BallGenerator.h"

class Aimer
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	bool _canShoot;
	int _shootTime;
	float _angle;
	Texture _spritesheet;
	Sprite *_sprite, *_heldBall;
	//Soon(TM)
	//BallGenerator *_bgen;
};

