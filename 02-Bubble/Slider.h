#pragma once
#include "Button.h"
class Slider :
	public Sprite
{
public:
	Slider(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	void init(glm::vec2 minPos, glm::vec2 maxPos, glm::vec2 spriteStartingPos, float currentValue);
	
	void select();
	void unselect();
	void unselectMouse();

	//Perform all checks for mouse interaction
	void updateDrag();
	void checkMouseClick();
	bool checkMouseHover();

	//Returns whether other hover updates should be ignored
	bool beingDragged();

	//Right and left click
	void moveRight();
	void moveLeft();

	//Get ratio
	float updateValueFromPos();
	//Set ratios
	void updatePosFromValue(float audioValue);

private:
	bool _dragged, _selected;
	glm::vec2 _minPos, _maxPos;

	glm::vec2 _spritesheet_pos, _spritesheet_size;
	glm::vec2 _position, _size;

	Texture *_spritesheet;
};

