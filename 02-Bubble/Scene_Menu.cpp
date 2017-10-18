#include <iostream>
#include <algorithm>


#include "Scene_Menu.h"
#include "../Game.h"

#define BUTTON_SIZE glm::vec2(128.f, 64.f)
#define BUTTON_SPRITESHEET_SIZE glm::vec2(0.5f, 0.25f)
#define MOVE_COOLDOWN 50


void Scene_Menu::init()
{
	initShaders();

	_selectedButton = 0;
	_state = NONE;

	_b_Texture = new Texture();
	if (!_b_Texture->loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load spritesheet.png");
	}


	_selectedButton = 0;

	//Play button
	_b_play = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_play->setPosition(glm::vec2((SCREEN_WIDTH-BUTTON_SIZE.x)/2, (SCREEN_HEIGHT-BUTTON_SIZE.y)/2 + BUTTON_SIZE.y*1));
		_b_play->setTexturePosition(glm::vec2(0.125f,0.f + BUTTON_SPRITESHEET_SIZE.y * 1));

	//Options button
	_b_options = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_options->setPosition(glm::vec2((SCREEN_WIDTH - BUTTON_SIZE.x) / 2, (SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 1));
		_b_options->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 2));

	//Exit button
	_b_exit = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_exit->setPosition(glm::vec2((SCREEN_WIDTH - BUTTON_SIZE.x) / 2, (SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 1));
		_b_exit->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 3));

	play();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene_Menu::update(int deltaTime)
{
	currentTime += deltaTime;

	if (Game::instance().getSpecialKey(13)) { /*Enter key pressed*/
		switch (_selectedButton) {
			case 0: play();
			case 1: openOptions();
			case 2: exit();
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && _moveCooldown == 0) {
		_selectedButton -= 1;
		_selectedButton %= 3;
		_moveCooldown = MOVE_COOLDOWN;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && _moveCooldown == 0) {
		_selectedButton += 1;
		_selectedButton %= 3;
		_moveCooldown = MOVE_COOLDOWN;
	}
	_moveCooldown = std::max(_moveCooldown - deltaTime, 0);
}

void Scene_Menu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_b_play->render();
	_b_options->render();
	_b_exit->render();
}

int Scene_Menu::getState()
{
	return _state;
}





void Scene_Menu::play()
{
	_state = PLAYING;
}

void Scene_Menu::openOptions()
{
	_state = OPTIONS;
}

void Scene_Menu::exit()
{
	_state = EXIT;
}
/*
bool Scene_Menu::wantsToQuit()
{
	return _wantsToQuit;
}
*/