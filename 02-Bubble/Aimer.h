#pragma once

#include "../Sprite.h"

#include "Ball_Held.h"
#include "BallManager.h"

class Aimer
{

public:
	Aimer() {}
	void init(const glm::vec2 &pos, glm::vec2 &minRenderCoords, ShaderProgram &shaderProgram, BallManager *bmng);
	//Updates aimer rotation and ball position. Asks for new ball if needed
	void update(int deltaTime);
	void render();

private:
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
};

