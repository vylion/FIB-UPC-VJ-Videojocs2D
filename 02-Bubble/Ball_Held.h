#ifndef _HELDBALL_INCLUDE
#define _HELDBALL_INCLUDE

#include "Ball.h"

class Ball_Held :
	public Ball
{
public:
	Ball_Held(Texture *spritesheet, ShaderProgram & shaderProgram, Ball * b, const glm::vec2 &aimerPos, glm::vec2 & aimerSize);
	
	using Ball::update;
	void update(int deltaTime, float &angle);

private:
	glm::vec2 _aimerPos, _aimerSize;
};

#endif