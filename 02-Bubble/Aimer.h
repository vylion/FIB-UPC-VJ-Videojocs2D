#pragma once

#include "../Sprite.h"

#include "Ball_Held.h"
#include "BallManager.h"

class Aimer
{

public:
	enum state {READY, SHOOTING, SWAPPING, LOADING, LAUNCHED_BALL, W8_BALL_MANAGER };

	Aimer() {}
	void init(const glm::vec2 &cannonPos, glm::vec2 &minRenderCoords, ShaderProgram &shaderProgram, BallManager *bmng);
	//Updates aimer rotation and ball position. Asks for new ball if needed
	void update(int deltaTime, int &bmngState);
	void render();

private:
	int _state, _animationTime;

	float _angle, _animationRatio;
	glm::vec2 _cannonPosition, _minRenderCoords;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;
	Sprite *_cannon, *_mainBox, *_swapBox;
	Ball_Held *_heldBall, *_swappedBall, *_nextBall;
	BallManager *_bmng;

	//Asks for new ball to ballManager
	void getNewHeldBall();
	//Check key presses
	void checkButtons(int deltaTime);
	//Animate ball load
	void loadUpdate(int deltaTime);
	void shootUpdate(int deltaTime);
	void swapUpdate(int deltaTime);

	glm::vec2 calculateLaunchedBallPosition();
};

