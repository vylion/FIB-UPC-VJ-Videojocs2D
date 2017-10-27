#pragma once
#include <functional>
#include "..\Sprite.h"

class Button :
	public Sprite
{
public:
	Button(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);
	
	//Position in screen, position in spritesheet, callback function
	void init(glm::vec2 buttonPos, glm::vec2 spriteStartingPos);

	void setCallback(std::function<void(void)> callback);

	//Change displayed texture
	void select(); 	void unselect();

	//Trigger callback
	void use();

private:
	std::function<void(void)> _callback;
	glm::vec2 _spritesheet_pos, _spritesheet_size;
};

