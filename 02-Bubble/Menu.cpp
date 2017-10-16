#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include "Menu.h"
#include "..\Game.h"

#define BUTTON_SIZE glm::vec2(128.f, 64.f)
#define BUTTON_SPRITESHEET_SIZE glm::vec2(0.5f, 0.25f)

Menu::Menu()
{
	_scene = Scene();
}

void Menu::init()
{
	initShaders();
	_selectedButton = 0;
	_wantsToQuit = false;

	_b_Texture = new Texture();
	if (!_b_Texture->loadFromFile("../media/images/spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load spritesheet.png");
	}


	_selectedButton = 0;

	//Play button
	_b_play = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &_texProgram);
		_b_play->setPosition(glm::vec2((SCREEN_WIDTH-BUTTON_SIZE.x)/2, (SCREEN_HEIGHT-BUTTON_SIZE.y)/2 + BUTTON_SIZE.y*1));
		_b_play->setTexturePosition(glm::vec2(0.125f,0.f + BUTTON_SPRITESHEET_SIZE.y * 1));

	//Options button
	_b_options = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &_texProgram);
		_b_options->setPosition(glm::vec2((SCREEN_WIDTH - BUTTON_SIZE.x) / 2, (SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 1));
		_b_options->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 2));

	//Exit button
	_b_exit = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &_texProgram);
		_b_exit->setPosition(glm::vec2((SCREEN_WIDTH - BUTTON_SIZE.x) / 2, (SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 1));
		_b_exit->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 3));

	play();
}

void Menu::update(int deltaTime)
{
	if (_playing)
		_scene.update(deltaTime);
	else {
		if (Game::instance().getSpecialKey(13)) { /*Enter key pressed*/
			switch (_selectedButton) {
				case 0: play();
				case 1: openOptions();
				case 2: exit();
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			_selectedButton -= 1;
			_selectedButton %= 3;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			_selectedButton += 1;
			_selectedButton %= 3;
		}
	}
}

void Menu::render()
{
	if (_playing)
		_scene.render();
	else {
		_b_play->render();
		_b_options->render();
		_b_exit->render();
	}
}



void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << std::endl;
		std::cout << "" << vShader.log() << std::endl << std::endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << std::endl;
		std::cout << "" << fShader.log() << std::endl << std::endl;
	}
	_texProgram.init();
	_texProgram.addShader(vShader);
	_texProgram.addShader(fShader);
	_texProgram.link();
	if (!_texProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << std::endl;
		std::cout << "" << _texProgram.log() << std::endl << std::endl;
	}
	_texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Menu::play()
{
	_playing = true;
	int level = 1;
	_scene.init(_texProgram, level);
}

void Menu::openOptions()
{
}

void Menu::exit()
{
	_wantsToQuit = true;
}

bool Menu::wantsToQuit()
{
	return _wantsToQuit;
}