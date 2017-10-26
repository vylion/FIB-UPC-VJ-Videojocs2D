#pragma once
#include "..\Sprite.h"

class Button :
	public Sprite
{
public:
	Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	
	void setCallback(void (*callback)(void));
	void init(glm::vec2 spriteStartingPos);

	void select();
	void unselect();

	void use();
private:
	void *_callback;
	glm::vec2 _spritesheet_pos, _spritesheet_size;
};

