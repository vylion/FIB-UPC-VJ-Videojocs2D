#pragma once

#include "../Sprite.h"

#include "Ball_Held.h"
#include "BallManager.h"

class Aimer
{

public:
	Aimer() {}
	void init(const glm::vec2 &pos, glm::vec2 &minRenderCoords, ShaderProgram &shaderProgram, BallManager *bmng);
	void update(int deltaTime);
	void render();

private:

	bool _canShoot;
	int _shootTime;
	float _angle;
	glm::vec2 _position, _size, _minRenderCoords;
	Texture *_spritesheet;
	Sprite *_sprite;
	Ball_Held *_heldBall;
	BallManager *_bmng;

	void ballToHeldball(Ball *b);
};

