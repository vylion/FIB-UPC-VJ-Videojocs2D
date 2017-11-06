#include "Slider.h"
#include "../Game.h"

Slider::Slider(const glm::vec2 & quadSize, const glm::vec2 & sizeInSpritesheet, Texture * spritesheet, ShaderProgram * program)
	: Sprite(quadSize, sizeInSpritesheet, spritesheet, program)
{
	_spritesheet = spritesheet;
	_size = quadSize;
	_spritesheet_size = sizeInSpritesheet / _spritesheet->getSize();
}

void Slider::init(glm::vec2 minPos, glm::vec2 maxPos, glm::vec2 spriteStartingPos, float currentValue)
{
	_minPos = minPos;
	_selected = false;
	_maxPos = glm::vec2(maxPos.x-_size.x, _minPos.y + _size.y);
	_dragged = false;

	updatePosFromValue(currentValue);

	_spritesheet_pos = spriteStartingPos;
	setTexturePosition(_spritesheet_pos);
}

void Slider::select()
{
	if (!_selected) {

		setTexturePosition(_spritesheet_pos + glm::vec2(0, _spritesheet_size.y));
		_selected = true;
	}
}

void Slider::unselect()
{
	_selected = false;
	setTexturePosition(_spritesheet_pos);
}

void Slider::unselectMouse()
{
	if (!checkMouseHover()) unselect();
}

void Slider::updateDrag()
{
	//Mouse still pressed
	if (!Game::instance().mouseJustReleased()) {
		//Update position
		_position.x = Game::instance().getMousePos().x - _size.x / 2.f;
		_position.x = max(_minPos.x, min(_position.x, _maxPos.x));
		setPosition(_position);
	}
	//Mouse released
	else {
		_dragged = false;
	}
}

void Slider::checkMouseClick()
{
	glm::vec2 currentMousePos = Game::instance().getMousePos();
	//Mouse is on top of us
	if (currentMousePos.x >= _minPos.x && currentMousePos.y >= _minPos.y && currentMousePos.x <= _maxPos.x && currentMousePos.y <= _maxPos.y) {
		//Click
		if (Game::instance().mouseJustPressed()) {
			_dragged = true;
		}
	}
}

bool Slider::checkMouseHover()
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

bool Slider::beingDragged()
{
	return _dragged;
}

void Slider::moveRight()
{
	float currentValue = updateValueFromPos();
	float newValue = min(currentValue + 0.1f, 1.f);
	updatePosFromValue(newValue);
}

void Slider::moveLeft()
{
	float currentValue = updateValueFromPos();
	float newValue = max(currentValue - 0.1f, 0.f);
	updatePosFromValue(newValue);
}

float Slider::updateValueFromPos()
{
	float ret = ((_position.x+_size.x/2.f) - _minPos.x) / (_maxPos.x - _minPos.x);
	return ret;
}

void Slider::updatePosFromValue(float posRatio)
{
	float posx = _minPos.x + posRatio * (_maxPos.x - _minPos.x);
	_position.x = posx;
	_position.x = max(_minPos.x, min(_position.x, _maxPos.x - _size.x));
	_position.y = _minPos.y;
	setPosition(_position);
}
