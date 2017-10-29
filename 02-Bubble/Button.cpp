#include "Button.h"
#include "Scene_Menu.h"
#include "Pause.h"

#define CHANGE_BUTTON_SFX "../media/audio/sounds/menu_button_change.ogg"
#define CLICK_BUTTON_SFX "../media/audio/sounds/menu_button_click.ogg"


Button::Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
	: Sprite(quadSize, sizeInSpritesheet, spritesheet, program)
{
	_spritesheet_size = sizeInSpritesheet / spritesheet->getSize();
}


void Button::init(glm::vec2 buttonPos, glm::vec2 spriteStartingPos)//, void(Scene_Menu::*callback)(void))
{
	setPosition(buttonPos);
	_spritesheet_pos = spriteStartingPos;
	setTexturePosition(spriteStartingPos);
	//_callback = callback;
}

void Button::setCallback(std::function<void(void)> callback)
{
	_callback = callback;
}

void Button::select(bool playSound)
{
	if (playSound) SoundManager::instance().playSound(CHANGE_BUTTON_SFX);
	setTexturePosition(_spritesheet_pos + glm::vec2(0, _spritesheet_size.y));
}

void Button::unselect()
{
	setTexturePosition(_spritesheet_pos);
}

void Button::use()
{
	_callback();
}