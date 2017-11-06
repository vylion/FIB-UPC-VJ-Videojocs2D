#pragma once
#include <functional>
#include "..\Sprite.h"

class Button :
	public Sprite
{
public:
	Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	
	//Position in screen, position in spritesheet
	void init(glm::vec2 buttonPos, glm::vec2 spriteStartingPos);

	void setCallback(std::function<void(void)> callback);

	bool checkMouseHover();

	//Change displayed texture
	void select(bool playSound=true);
	void unselect();
	void unselectMouse();

	//Trigger callback
	void use();

protected:
	std::function<void(void)> _callback;
	glm::vec2 _spritesheet_pos, _spritesheet_size;
	glm::vec2 _position, _size;

	bool _selected;

	Texture *_spritesheet;
};

