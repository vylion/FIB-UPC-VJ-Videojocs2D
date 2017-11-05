#include "Button.h"
#include "Scene_Menu.h"
#include "Pause.h"
#include "../Game.h"

#define CHANGE_BUTTON_SFX "../media/audio/sounds/menu_button_change.ogg"
#define CLICK_BUTTON_SFX "../media/audio/sounds/menu_button_click.ogg"


Button::Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
	: Sprite(quadSize, sizeInSpritesheet, spritesheet, program)
{
	_spritesheet = spritesheet;
	_size = sizeInSpritesheet;
	_spritesheet_size = sizeInSpritesheet / _spritesheet->getSize();
}


void Button::init(glm::vec2 buttonPos, glm::vec2 spriteStartingPos)
{
	_position = buttonPos;
	setPosition(_position);
	_spritesheet_pos = spriteStartingPos;
	setTexturePosition(spriteStartingPos);
	_selected = false;
}

void Button::setCallback(std::function<void(void)> callback)
{
	_callback = callback;
}

bool Button::checkMouseHover()
{
	glm::vec2 mousePos = Game::instance().getMousePos();
	float minx, maxx, miny, maxy;
	minx = _position.x;
	maxx = _position.x + _size.x;
	miny = _position.y;
	maxy = _position.y + _size.y;

	bool hovering = (mousePos.x <= maxx && mousePos.x >= minx && mousePos.y <= maxy && mousePos.y >= miny);

	return hovering;
}

void Button::select(bool playSound)
{
	if (!_selected) {
		if (playSound) SoundManager::instance().playSound(CHANGE_BUTTON_SFX);
		setTexturePosition(_spritesheet_pos + glm::vec2(0, _spritesheet_size.y));
		_selected = true;
	}
}

void Button::unselect()
{
	_selected = false;
	setTexturePosition(_spritesheet_pos);
}

void Button::unselectMouse()
{
	if (!checkMouseHover()) unselect();
}

void Button::use()
{
	SoundManager::instance().playSound(CLICK_BUTTON_SFX);
	_callback();
}