#include <iostream>
#include <algorithm>


#include "Scene_Menu.h"
#include "../Game.h"
#include "Pause.h"


#define FADE_IN_TIME 750.f
#define FADE_OUT_TIME 150.f

#define LOGO_SIZE glm::vec2(384.f, 256.f)
#define LOGO_SPRITESHEET_SIZE glm::vec2(0.6f, 1.f)

#define BUTTON_SIZE glm::vec2(256.f, 32.f)
#define BUTTON_SPRITESHEET_SIZE glm::vec2(0.4f, 0.125f)

#define SHORT_BUTTON_SIZE glm::vec2(128.f, 32.f)
#define SHORT_BUTTON_SPRITESHEET_SIZE glm::vec2(0.2f, 0.125f)

#define BUTTON_MOVE_COOLDOWN 175

#define MENU_SPRITES "media/images/menu_sprites.png"
#define MENU_BACKGROUND "media/images/menu_bg.png"
#define MUSIC_FILE "media/audio/music/menu_bgm.ogg"


void Scene_Menu::init()
{
	initShaders();
	Pause::instance().init(&texProgram);
	initAudio();

	_state = FADE_IN;
	_fadeTime = 0;

	/*----------------------------------------TEXTURES--------------------------------------------------*/

	_b_Texture = new Texture();
	if (!_b_Texture->loadFromFile(MENU_SPRITES, TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load menu_sprites.png");
	}
	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(MENU_BACKGROUND, TEXTURE_PIXEL_FORMAT_RGBA)) {
		printf("Couldn't load menu_bg.png");
	}


	/*----------------------------------------BUTTONS----------------------------------------------------*/
	_selectedButton = 0; //Play
	_halfButtonSide = 0; //Left
	_moveCooldown = 0;

	//Play button
	_b_play = new Button(BUTTON_SIZE, BUTTON_SIZE, _b_Texture, &texProgram);
	_b_play->init(glm::vec2((
		SCREEN_WIDTH - BUTTON_SIZE.x) / 2,
		(SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 1 + 50.f
	),
		glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 0)));
	_b_play->setCallback([this](void) { play(); });

	//How to play button
	_b_howtoplay = new Button(SHORT_BUTTON_SIZE, SHORT_BUTTON_SIZE, _b_Texture, &texProgram);
	_b_howtoplay->init(glm::vec2(
		SCREEN_WIDTH/2 - SHORT_BUTTON_SIZE.x,
		(SCREEN_HEIGHT - SHORT_BUTTON_SIZE.y) / 2 + SHORT_BUTTON_SIZE.y * 2.2f + 50.f
	), glm::vec2(0.6f, SHORT_BUTTON_SPRITESHEET_SIZE.y * (2 * 1)));
	_b_howtoplay->setCallback([this](void) { howtoplay(); });

	//Credits button
	_b_credits = new Button(SHORT_BUTTON_SIZE, SHORT_BUTTON_SIZE, _b_Texture, &texProgram);
	_b_credits->init(glm::vec2(
		SCREEN_WIDTH/2,
		(SCREEN_HEIGHT - SHORT_BUTTON_SIZE.y) / 2 + SHORT_BUTTON_SIZE.y * 2.2f + 50.f
	), glm::vec2(0.8f, SHORT_BUTTON_SPRITESHEET_SIZE.y * (2 * 1)));
	_b_credits->setCallback([this](void) { credits(); });

	//Options button
	_b_options = new Button(BUTTON_SIZE, BUTTON_SIZE, _b_Texture, &texProgram);
	_b_options->init(glm::vec2(
		(SCREEN_WIDTH - BUTTON_SIZE.x) / 2,
		(SCREEN_HEIGHT - BUTTON_SIZE.y) / 2 + BUTTON_SIZE.y * 3.4f + 50.f
	), glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 2)));
	_b_options->setCallback([this](void) { options(); });


	//Exit button
	_b_exit = new Button(BUTTON_SIZE, BUTTON_SIZE, _b_Texture, &texProgram);
	_b_exit->init(glm::vec2(
		(SCREEN_WIDTH - BUTTON_SIZE.x) / 2.f,
		(SCREEN_HEIGHT - BUTTON_SIZE.y) / 2.f + BUTTON_SIZE.y * 4.6f + 50.f
	), glm::vec2(0.6f, BUTTON_SPRITESHEET_SIZE.y * (2 * 3)));
	_b_exit->setCallback([this](void) {quit(); });



	_buttons.push_back(_b_play);
	_buttons.push_back(_b_howtoplay);
	_buttons.push_back(_b_credits);
	_buttons.push_back(_b_options);
	_buttons.push_back(_b_exit);
	bool playSound = false;
	_buttons[_selectedButton]->select(playSound);

	/*----------------------------------------LOGO-------------------------------------------------------*/

	_logo = Sprite::createSprite(LOGO_SIZE, LOGO_SIZE, _b_Texture, &texProgram);
		_logo->setPosition(glm::vec2((SCREEN_WIDTH - LOGO_SIZE.x) / 2.f, (SCREEN_HEIGHT - LOGO_SIZE.y) / 2.f + 5.f));
		//_b_exit->setTexturePosition(glm::vec2(0.125f, 0.f + BUTTON_SPRITESHEET_SIZE.y * 3));
		_logo->setTexturePosition(glm::vec2(0.f));

	/*----------------------------------------BACKGROUND-------------------------------------------------*/

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);


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
			SoundManager::instance().setMusicVolume(_fadeTime / FADE_IN_TIME);
			//Fade in has finished. Prepare fade with time for fade_out and update state
			if (_fadeTime >= FADE_IN_TIME) {
				_fadeTime = (int)FADE_OUT_TIME;
				_state = RUNNING;
			}
			//No break because we want to check buttons aswell
		case RUNNING:
			checkButtons(deltaTime);
			checkMouse();
			break;
		case FADE_OUT:
			_fadeTime -= deltaTime;
			//Decrease music according to fade level
			SoundManager::instance().setMusicVolume(max(0,_fadeTime) / FADE_OUT_TIME * 0.4f);
			//Fade out has finished. We can safely start the level now
			if (_fadeTime <= 0) {
				_fadeTime = 0;
				SoundManager::instance().pauseMusic(true);
				_state = OPEN_LEVEL;
			}
			break;
		case PAUSED:
			//Give control to pause menu
			switch (Pause::instance().update(deltaTime)) {
				case Pause::CONTINUE:
					_state = RUNNING;
					break;
				case Pause::QUIT:
					_state = RUNNING;
					break;
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
		case PAUSED:
			alpha = 0.5f;
			break;
	}
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();

	_logo->render();
	for (unsigned int i = 0; i < _buttons.size(); ++i) _buttons[i]->render();

	if (_state == PAUSED) Pause::instance().render();
}

void Scene_Menu::initAudio()
{
	SoundManager::instance().setMusic(MUSIC_FILE);
}

void Scene_Menu::checkButtons(int deltaTime)
{
	//Reduce moveCooldown regardless of situation
	_moveCooldown = std::max(_moveCooldown - deltaTime, 0);


	//Up key pressed
	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && _moveCooldown == 0) {
		_buttons[_selectedButton]->unselect();
		//Increase x-1 mod x to loop around and move -1. -1 mod x crashes badly
		_selectedButton = (_selectedButton + (_buttons.size() - 1)) % _buttons.size();
		if (_selectedButton == 2) _selectedButton = 1 + _halfButtonSide;
		else if (_selectedButton == 1) _selectedButton--;
		//Increase move cooldown
		_moveCooldown = BUTTON_MOVE_COOLDOWN;
		_buttons[_selectedButton]->select();
	}
	//Down key pressed
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && _moveCooldown == 0) {
		_buttons[_selectedButton]->unselect();
		_selectedButton = (_selectedButton + 1) % _buttons.size();
		if (_selectedButton == 1) _selectedButton = 1 + _halfButtonSide;
		else if (_selectedButton == 2) _selectedButton++;
		_moveCooldown = BUTTON_MOVE_COOLDOWN;
		_buttons[_selectedButton]->select();
	}
	//Neither is pressed so move cooldown is set 0. Enables swifter changing than holding down
	else if (Game::instance().getSpecialKeyReleased(GLUT_KEY_UP) || Game::instance().getSpecialKeyReleased(GLUT_KEY_DOWN)) {
		_moveCooldown = 0;
	}

	//Move buttons right to left if we are in the split row
	if (_moveCooldown == 0 && (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT)) && (_selectedButton == 1 || _selectedButton == 2)) {
		_buttons[_selectedButton]->unselect();
		_halfButtonSide = (_halfButtonSide + 1) % 2;
		_selectedButton = 1 + _halfButtonSide;
		_moveCooldown = BUTTON_MOVE_COOLDOWN;
		_buttons[_selectedButton]->select();
	}

	//M stands for music
	if (Game::instance().getKeyJustPressed('m')) {
		SoundManager::instance().toggleMusicPause();
	}

	//Enter key pressed
	if (Game::instance().getKeyJustPressed(13)) {
		_buttons[_selectedButton]->use();
	}
	//Escape from game
	else if (Game::instance().getKeyReleased(27)) {
		_state = state::EXIT;
	}
}

void Scene_Menu::checkMouse()
{
	if (Game::instance().mouseJustMoved()) {
		unsigned int i = 0;
		bool hoveringButton = false;

		while (i < _buttons.size() && !hoveringButton) {
			hoveringButton = _buttons[i]->checkMouseHover();
			if (!hoveringButton) i++;
		}
		if (hoveringButton) {
			_buttons[_selectedButton]->unselectMouse();
			_buttons[i]->select();
			_selectedButton = i;
			if (_selectedButton == 1) _halfButtonSide = 0;
			else if (_selectedButton == 2) _halfButtonSide = 1;
		}
	}
	if (Game::instance().mouseJustPressed()) {
		unsigned int i = 0;
		bool hoveringButton = false;

		while (i < _buttons.size() && !hoveringButton) {
			hoveringButton = _buttons[i]->checkMouseHover();
			if (!hoveringButton) i++;
		}
		if (hoveringButton) {
			_buttons[i]->use();
		}
	}
}

void Scene_Menu::play()
{
	_state = state::FADE_OUT;
	SoundManager::instance().dropAll();
}

void Scene_Menu::howtoplay()
{
	_state = state::HOW_TO_PLAY;
	SoundManager::instance().dropAll();
}

void Scene_Menu::credits()
{
	_state = state::OPEN_CREDITS;
	SoundManager::instance().dropAll();
}

void Scene_Menu::options()
{
	Pause::instance().init(&texProgram);
	_state = state::PAUSED;
	//SoundManager::instance().pauseMusic(true);
}

void Scene_Menu::quit()
{
	_state = state::EXIT;
}
