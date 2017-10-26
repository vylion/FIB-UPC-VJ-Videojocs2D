#include "Button.h"



Button::Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
	: Sprite(quadSize, sizeInSpritesheet, spritesheet, program)
{
	_spritesheet_size = sizeInSpritesheet;
}



void Button::setCallback(void (*callback)(void))
{
	_callback = callback;
}

void Button::select()
{
	setTexturePosition(_spritesheet_pos + glm::vec2(0, _spritesheet_size.y));
}

void Button::unselect()
{
}

void Button::use()
{
	void *_callback();
}