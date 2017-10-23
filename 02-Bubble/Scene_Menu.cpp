#include <iostream>
#include <algorithm>


#include "Scene_Menu.h"
#include "../Game.h"


#define FADE_IN_TIME 750.f
#define FADE_OUT_TIME 150.f

#define BUTTON_SIZE glm::vec2(256.f, 32.f)
#define BUTTON_SPRITESHEET_SIZE glm::vec2(0.4f, 0.125f)
#define LOGO_SIZE glm::vec2(384.f, 256.f)
#define LOGO_SPRITESHEET_SIZE glm::vec2(0.6f, 1.f)
#define BUTTON_MOVE_COOLDOWN 175

#define MUSIC_FILE "../media/audio/music/menu_bgm.ogg"
#define CHANGE_BUTTON_SFX "../media/audio/sounds/menu_button_change.ogg"
#define CLICK_BUTTON_SFX "../media/audio/sounds/menu_button_click.ogg"


void Scene_Menu::init()
{
	initShaders();

	_state = FADE_IN;
	_fadeTime = 0;

	/*----------------------------------------TEXTURES--------------------------------------------------*/

	_b_Texture = new Texture();
	if (!_b_Texture->loadFromFile("../media/images/menu_sprites.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load menu_sprites.png");
	}
	_bg_Texture = new Texture();
	if (!_bg_Texture->loadFromFile("../media/images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load menu_bg.png");
	}


	/*----------------------------------------BUTTONS----------------------------------------------------*/
	_selectedButton = 0; //Play

	//Play button
	_b_play = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_play->setPosition(glm::vec2((
			SCREEN_WIDTH-BUTTON_SIZE.x)/2,
			(SCREEN_HEIGHT-BUTTON_SIZE.y)/2 + BUTTON_SIZE.y * 1 + 50.f
		));
		_b_play->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 0)));
	//Options button
	_b_options = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_options->setPosition(glm::vec2(
			(SCREEN_WIDTH - BUTTON_SIZE.x) / 2,
			(SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 2.5 + 50.f
		));
		_b_options->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 1)));
	//Exit button
	_b_exit = Sprite::createSprite(BUTTON_SIZE, BUTTON_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_b_exit->setPosition(glm::vec2(
			(SCREEN_WIDTH - BUTTON_SIZE.x) / 2.f,
			(SCREEN_HEIGHT - BUTTON_SIZE.y) / 2.f + BUTTON_SIZE.y * 4 + 50.f
		));
		_b_exit->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 2)));

		
	_buttons.push_back(_b_play);
	_buttons.push_back(_b_options);
	_buttons.push_back(_b_exit);
	_buttons[_selectedButton]->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y));

	/*----------------------------------------LOGO-------------------------------------------------------*/

	_logo = Sprite::createSprite(LOGO_SIZE, LOGO_SPRITESHEET_SIZE, _b_Texture, &texProgram);
		_logo->setPosition(glm::vec2((SCREEN_WIDTH - LOGO_SIZE.x) / 2.f, (SCREEN_HEIGHT - LOGO_SIZE.y) / 2.f + 5.f));
		//_b_exit->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 3));
		_logo->setTexturePosition(glm::vec2(0.f));

	/*----------------------------------------BACKGROUND-------------------------------------------------*/

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f),  _bg_Texture, &texProgram);

	/*----------------------------------------MUSIC--------------------------------------------------------*/
	//SoundManager::instance().setMusic(MUSIC_FILE);
	//SoundManager::instance().setMusicVolume(0.f);

	SoundManager::instance().addSound(CHANGE_BUTTON_SFX);
	SoundManager::instance().setSoundVolume(CHANGE_BUTTON_SFX, 0.1f);
	SoundManager::instance().addSound(CLICK_BUTTON_SFX);

	/*----------------------------------------END--------------------------------------------------------*/

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

int Scene_Menu::update(int deltaTime)
{
	currentTime += deltaTime;

	//Contol fades
	switch (_state) {
		case FADE_IN:
			_fadeTime += deltaTime;
			//Increase music according to fade level
			SoundManager::instance().setMusicVolume(_fadeTime / FADE_IN_TIME * 0.6f);
			//Fade in has finished. Prepare fade with time for fade_out and update state
			if (_fadeTime >= FADE_IN_TIME) {
				_fadeTime = (int)FADE_OUT_TIME;
				_state = RUNNING;
			}
			//No break because we want to check buttons aswell
		case RUNNING:
			checkButtons(deltaTime);
			break;
		case FADE_OUT:
			_fadeTime -= deltaTime;
			//Decrease music according to fade level
			SoundManager::instance().setMusicVolume(max(0,_fadeTime) / FADE_OUT_TIME * 0.6f);
			//Fade out has finished. We can safely start the level now
			if (_fadeTime <= 0) {
				_fadeTime = 0;
				_state = OPEN_LEVEL;
			}
			break;
		default:
			break;
	}

	//Float effect for logo
	_logo->setPosition(glm::vec2(
		(SCREEN_WIDTH - LOGO_SIZE.x) / 2.f,
		(SCREEN_HEIGHT - LOGO_SIZE.y) / 2.f + 2*sin(currentTime/500) - 95.f
	));

	return _state;
}

void Scene_Menu::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	float alpha = 1.f;
	switch (_state) {
		case FADE_IN:
			alpha = (float)_fadeTime / FADE_IN_TIME;
			break;
		case FADE_OUT:
			alpha = (float)_fadeTime / FADE_OUT_TIME;
			break;
	}
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();

	_logo->render();

	_b_play->render();
	_b_options->render();
	_b_exit->render();
}

int Scene_Menu::getLevelToOpen()
{
	return 1;
}

void Scene_Menu::checkButtons(int deltaTime)
{
	//Reduce moveCooldown regardless of situation
	_moveCooldown = std::max(_moveCooldown - deltaTime, 0);


	//Up key pressed
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && _moveCooldown == 0) {
		//Unselect current button. Texture update
		_buttons[_selectedButton]->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * _selectedButton)));
		//Increase x-1 mod x to loop around and move -1. -1 mod x crashes badly
		_selectedButton = (_selectedButton + (_buttons.size() - 1)) % _buttons.size();
		//Increase move cooldown
		_moveCooldown = BUTTON_MOVE_COOLDOWN;
		//Select new button. Texture update
		_buttons[_selectedButton]->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * _selectedButton + 1)));

		//Play button move sound
		SoundManager::instance().playSound(CHANGE_BUTTON_SFX);
	}
	//Down key pressed
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && _moveCooldown == 0) {
		_buttons[_selectedButton]->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * _selectedButton)));
		_selectedButton = (_selectedButton + 1) % _buttons.size();
		_moveCooldown = BUTTON_MOVE_COOLDOWN;
		_buttons[_selectedButton]->setTexturePosition(glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * _selectedButton + 1)));

		SoundManager::instance().playSound(CHANGE_BUTTON_SFX);
	}
	//Neither is pressed so move cooldown is set 0. Enables swifter changing than holding down
	else if (Game::instance().getSpecialKeyReleased(GLUT_KEY_UP) || Game::instance().getSpecialKeyReleased(GLUT_KEY_DOWN)) {
		_moveCooldown = 0;
	}

	//Enter key pressed
	if (Game::instance().getKey(13)) {
		SoundManager::instance().playSound(CLICK_BUTTON_SFX);
		switch (_selectedButton) {
			case 0: //Play
				_state = FADE_OUT;
				break;
			case 1: //Options
					//_state = RUNNING;
					//#include <shenanigans>
				break;
			case 2: //Exit
				_state = EXIT;
				break;
		}
	}
	//Escape from game
	else if (Game::instance().getKeyReleased(27)) {
		_state = EXIT;
	}
}
