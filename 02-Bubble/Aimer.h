#ifndef _AIMER_INCLUDE
#define _AIMER_INCLUDE

#include "../Sprite.h"
//#include "../Game.h"
//Soon(TM)
#include "BallManager.h"

class Aimer
{

public:
	Aimer() {}
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, BallManager &bmng);
	void update(int deltaTime);
	void render();

private:

	bool _canShoot;
	int _shootTime;
	float _angle;
	Texture _spritesheet;
	Sprite *_sprite;
	//Ball *_heldBall;
	BallManager *_bmng;
};
#endif

