#ifndef _AIMER_INCLUDE
#define _AIMER_INCLUDE

#include "../Sprite.h"
//#include "../Game.h"
//Soon(TM)
//#include "BallManager.h"

class Aimer
{

public:
	Aimer() {}
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
	//BallManager *_bmng;
};
#endif

