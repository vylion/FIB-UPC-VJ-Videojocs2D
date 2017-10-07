#include "Ball_Held.h"


Ball_Held::Ball_Held(	Texture * spritesheet,
						ShaderProgram & shaderProgram,
						Ball * b,
						const glm::vec2 & aimerPos,
						glm::vec2 & aimerSize)
	:Ball(b->getSize(), b->getSpritesheetSize(), spritesheet, shaderProgram), _aimerPos(aimerPos), _aimerSize(aimerSize)
{
	setColor(b->getColor());
	setSize(glm::vec2(b->getSize().x/2,b->getSize().y/2));
}

void Ball_Held::update(int deltaTime, float &angle) {
	update(deltaTime);
}

