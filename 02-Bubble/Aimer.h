#pragma once

#include "../Sprite.h"

#include "Ball_Held.h"
#include "BallManager.h"

class Aimer
{

public:
	enum state {READY, ANIMATING, LAUNCHED_BALL, W8_BALL_MANAGER };

	Aimer() {}
	void init(const glm::vec2 &pos, glm::vec2 &minRenderCoords, Texture *tex,  ShaderProgram &shaderProgram, BallManager *bmng);
	//Updates aimer rotation and ball position. Asks for new ball if needed
	void update(int deltaTime, int &bmngState);
	void render();

private:
	int _state, _animationTime;

	float _angle;
	glm::vec2 _position, _minRenderCoords;
	Texture *_spritesheet;
	Sprite *_sprite;
	Ball_Held *_heldBall;
	BallManager *_bmng;

	//Asks for new ball to ballManager
	void getNewHeldBall();
	//Check key presses
	void checkButtons(int deltaTime);
	//Animate ball load
	void animationUpdate(int deltaTime);
};

