#include <algorithm>

#include "Pause.h"
#include "../Game.h"
#include "SoundManager.h"
#include "Button.h"

#include <GL/glut.h>

#define FADE_TIME 100.f
#define ROW_COOLDOWN 50
#define TEXTURE_FILE "media/images/pause_sprites.png"
#define TEXTURE_SIZE glm::vec2(384.f, 240.f)

#define PANEL_SIZE glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)
#define PANEL_SPRITESHEET_SIZE PANEL_SIZE
//#define PANEL_POSITION glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2) - PANEL_SIZE/2
//Translated to:
#define PANEL_POSITION glm::vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4)

#define CONTROLS_SIZE glm::vec2(12.f, 12.f)
#define CONTROLS_SPRITESHEET_SIZE glm::vec2(16.f,16.f)
#define CONTROLS_STARTING_POSITION PANEL_POSITION + glm::vec2(40.f, 88.f)
#define CONTROLS_SPACE glm::vec2(240, 36.f)

#define BUTTON_SIZE glm::vec2(64.f, 32.f)
#define BUTTON_PIXEL_SIZE BUTTON_SIZE
#define BUTTON_STARTING_POSITION PANEL_POSITION + glm::vec2(40.f, 182.f)
#define BUTTON_SPACE glm::vec2(22.f, 0.f)


void Pause::init(ShaderProgram* shaderProgram)
{
	_shaderProgram = shaderProgram;
	if (_texture == nullptr) {

		_texture = new Texture();
		if (!_texture->loadFromFile(TEXTURE_FILE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load pause texture");
	}


	/*----------------------------------------MUSIC-------------------------------------------------------*/

	//SoundManager::instance().pauseMusic(true);
	_masterVol = SoundManager::instance().getMasterVolume();
	_masterOffset = _masterVol;
	_musicVol = SoundManager::instance().getMusicVolume();
	_musicOffset = _musicVol;
	_soundVol = SoundManager::instance().getSoundVolumeFactor();
	_soundOffset = _soundVol;

	/*----------------------------------------BUTTONS-------------------------------------------------------*/
	//No buttons loaded
	if (!_buttons.size()) {
		_b_quit = new Button(BUTTON_SIZE, BUTTON_PIXEL_SIZE, _texture, _shaderProgram);
			_b_quit->init(BUTTON_STARTING_POSITION, glm::vec2(PANEL_SIZE.x, BUTTON_PIXEL_SIZE.y * 0) / TEXTURE_SIZE);
			_b_quit->setCallback([this](void) { toMenu(); });
		_b_cancel = new Button(BUTTON_SIZE, BUTTON_PIXEL_SIZE, _texture, _shaderProgram);
			_b_cancel->init(BUTTON_STARTING_POSITION + glm::vec2(BUTTON_SIZE.x, 0.f) + BUTTON_SPACE, glm::vec2(PANEL_SIZE.x, BUTTON_PIXEL_SIZE.y * 2) / TEXTURE_SIZE);
			_b_cancel->setCallback([this](void) { cancel(); });
		_b_confirm = new Button(BUTTON_SIZE, BUTTON_PIXEL_SIZE, _texture, _shaderProgram);
			_b_confirm->init(BUTTON_STARTING_POSITION + 2.f*(glm::vec2(BUTTON_SIZE.x, 0.f) + BUTTON_SPACE), glm::vec2(PANEL_SIZE.x, BUTTON_PIXEL_SIZE.y * 4) / TEXTURE_SIZE);
			_b_confirm->setCallback([this](void) { confirm(); });

		_buttons.push_back(_b_quit);
		_buttons.push_back(_b_cancel);
		_buttons.push_back(_b_confirm);
	}
	//Buttons loaded, unset all to simulate starting state
	else {
		for (unsigned int i = 0; i < _buttons.size(); ++i) _buttons[i]->unselect();
	}
	_selectedRow = 3;
	_selectedCol = 2;

	bool playSound = false;
	_buttons[_selectedCol]->select(playSound);


	/*----------------------------------------BACKGROUND----------------------------------------------------*/

	_panel = new Sprite(PANEL_SIZE, PANEL_SPRITESHEET_SIZE, _texture, _shaderProgram);
	_panel->setPosition(PANEL_POSITION);
	//No setTexturePosition because texture starts at 0,0

	/*----------------------------------------SLIDERS-------------------------------------------------------*/

	if (!_sliders.size()) {
		_masterVolumeMarker = new Slider(CONTROLS_SIZE, CONTROLS_SPRITESHEET_SIZE, _texture, _shaderProgram);
		_musicVolumeMarker = new Slider(CONTROLS_SIZE, CONTROLS_SPRITESHEET_SIZE, _texture, _shaderProgram);
		_soundVolumeMarker = new Slider(CONTROLS_SIZE, CONTROLS_SPRITESHEET_SIZE, _texture, _shaderProgram);

		_sliders.push_back(_masterVolumeMarker);
		_sliders.push_back(_musicVolumeMarker);
		_sliders.push_back(_soundVolumeMarker);
	}

	initSliders();

	/*----------------------------------------END----------------------------------------------------------*/

	_action = action::WAIT;
	_state = state::FADE_IN;

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	_fadeTime = 0.0f;
}

void Pause::clearAll()
{
	for (unsigned int i = 0; i < _buttons.size(); ++i) delete _buttons[i];
	_buttons.clear();
	for (unsigned int i = 0; i < _sliders.size(); ++i) delete _sliders[i];
	_sliders.clear();
	_action = action::WAIT;
	_state = state::RUNNING;
	_masterVol = _masterOffset = _musicVol = _musicOffset = _soundVol = _soundOffset = 0;
	delete _texture;
}

int Pause::update(int deltaTime)
{
	_fadeTime += (float)deltaTime;
	

	switch (_state) {
		case state::RUNNING:
			checkButtons(deltaTime);
			checkMouse();
			break;
		case state::FADE_IN:
			_ratio = _fadeTime / FADE_TIME;
			if (_ratio >= 1.f) {
				_state = state::RUNNING;
			}
			break;
		case state::FADE_OUT:
			_ratio = 1 - _fadeTime / FADE_TIME;
			if (_ratio <= 0.f) {
				_state = state::RETURNED;
			}
			break;
		case state::RETURNED:
			_action = action::WAIT;
			_state = state::FADE_IN;
			_fadeTime = 0;
			break;
	}
	return _action;
}

void Pause::render()
{

	_shaderProgram->use();
	_shaderProgram->setUniformMatrix4f("projection", projection);
	float fadeRatio = 1.f;
	switch (_state) {
		case FADE_IN:
			fadeRatio = (float)_fadeTime / FADE_TIME;
			break;
		case FADE_OUT:
			fadeRatio = (float)_fadeTime / FADE_TIME;
			break;
	}

	//_shaderProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f - fadeRatio * 0.6f);
	//Render alpha square

	_shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	//Move panel on top off screen limits
	modelview = glm::translate(modelview, glm::vec3(0.f, fadeRatio-(2*PANEL_SIZE.y + PANEL_POSITION.y), 0.f));
	_shaderProgram->setUniformMatrix4f("modelview", modelview);
	_shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	//Render panel, sliders and buttons
	_panel->render();
	for (unsigned int i = 0; i < _sliders.size(); ++i) _sliders[i]->render();
	for (unsigned int i = 0; i < _buttons.size(); ++i) _buttons[i]->render();
}

void Pause::initSliders()
{
	_masterOffset = SoundManager::instance().getMasterVolume();
	_musicOffset = SoundManager::instance().getMusicVolume();
	_soundOffset = SoundManager::instance().getSoundVolumeFactor();

	glm::vec2 minPos, maxPos, texPos;

	minPos = CONTROLS_STARTING_POSITION + 0.f * glm::vec2(0.f, CONTROLS_SPACE.y);
	maxPos = minPos + CONTROLS_SPACE;
	texPos = glm::vec2(PANEL_SIZE.x, 192.f);
	_masterVolumeMarker->init(minPos, maxPos, texPos/ TEXTURE_SIZE, _masterOffset);

	minPos = CONTROLS_STARTING_POSITION + 1.f * glm::vec2(0.f, CONTROLS_SPACE.y);
	maxPos = minPos + CONTROLS_SPACE;
	texPos = glm::vec2(PANEL_SIZE.x, 192.f);
	_musicVolumeMarker->init(minPos, maxPos, texPos / TEXTURE_SIZE, _musicOffset);

	minPos = CONTROLS_STARTING_POSITION + 2.f * glm::vec2(0.f, CONTROLS_SPACE.y);
	maxPos = minPos + CONTROLS_SPACE;
	texPos = glm::vec2(PANEL_SIZE.x, 192.f);
	_soundVolumeMarker->init(minPos, maxPos, texPos / TEXTURE_SIZE, _soundOffset);

}

void Pause::checkMouse()
{
	if (Game::instance().mouseJustMoved()) {
		//Dragging one slider so we will ignore all hovers
		if (_sliders[0]->beingDragged()){
			_sliders[0]->updateDrag();
			_masterOffset = _sliders[0]->updateValueFromPos();
			SoundManager::instance().setMasterVolume(_masterOffset);
		}
		else if (_sliders[1]->beingDragged()) {
			_sliders[1]->updateDrag();
			_musicOffset = _sliders[1]->updateValueFromPos();
			SoundManager::instance().setMusicVolume(_musicOffset);
		}
		else if (_sliders[2]->beingDragged()) {
			_sliders[2]->updateDrag();
			_soundOffset = _sliders[2]->updateValueFromPos();
			SoundManager::instance().setSoundVolumeFactor(_soundOffset);
		}
		//Check hovers
		else {
			bool hoveringButton = false;
			bool hoveringSlider = false;
			unsigned int i = 0;
			//Check until an entity is hovered or we finish checking all
			while (i < _buttons.size() && !hoveringButton && !hoveringSlider) {
				hoveringButton = _buttons[i]->checkMouseHover();
				hoveringSlider = _sliders[i]->checkMouseHover();
				if (!hoveringButton && !hoveringSlider) i++;
			}
			//Slider being hovered
			if (hoveringSlider) {
				//A button previously hovered

				if (_selectedRow == 3) _buttons[_selectedCol]->unselect();
				//If we are on a different slider
				if (_selectedRow != i) {
					if (_selectedRow != 3) _sliders[_selectedRow]->unselect();
					_sliders[i]->select();
					_selectedRow = i;
				}
				//Otherwise ignore
			}
			//Button being hovered
			else if (hoveringButton) {
				//A slider was previously hovered
				if (_selectedRow != 3) {
					_sliders[_selectedRow]->unselect();
					_selectedRow = 3;
					_buttons[i]->select();
				}
				//If we are on a different button
				_buttons[_selectedCol]->unselect();
				_buttons[i]->select(i != _selectedCol);
				_selectedCol = i;
				//Otherwise ignore
			}
		}
	}
	if (Game::instance().mouseJustPressed()) {
		unsigned int i = 0;
		bool hoveringButton = false;
		bool hoveringSlider = false;

		while (i < _buttons.size() && !hoveringButton && !hoveringSlider) {
			hoveringButton = _buttons[i]->checkMouseHover();
			_sliders[i]->checkMouseClick();
			hoveringSlider = _sliders[i]->beingDragged();
			if (!hoveringButton && !hoveringSlider) i++;
		}
		if (hoveringSlider) {

		}
		if (hoveringButton) {
			_buttons[i]->use();
		}
	}
}

void Pause::checkButtons(int deltaTime)
{
	_moveCooldown = max(_moveCooldown - deltaTime, 0);

	//No keys pressed
	if (_moveCooldown != 0.f && 
		(Game::instance().getSpecialKeyReleased(GLUT_KEY_UP)   ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_DOWN)  ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_LEFT))
		)
		_moveCooldown = 0;
	else {
		if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_UP)) {
			//
			if (_selectedRow < (int)_sliders.size()) _sliders[_selectedRow]->setTexturePosition(glm::vec2(PANEL_SIZE.x, 192.f) / TEXTURE_SIZE);
			else _buttons[_selectedCol]->unselect();

			_selectedRow = (_selectedRow + 3) % 4;
			_moveCooldown = ROW_COOLDOWN;

			if (_selectedRow < (int)_sliders.size()) _sliders[_selectedRow]->setTexturePosition(glm::vec2(PANEL_SIZE.x, 192.f + CONTROLS_SPRITESHEET_SIZE.y ) / TEXTURE_SIZE);
			else _buttons[_selectedCol]->select();
		}
		else if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_DOWN)) {
			if (_selectedRow < (int)_sliders.size())
				_sliders[_selectedRow]->setTexturePosition(glm::vec2(PANEL_SIZE.x, 192.f) / TEXTURE_SIZE);
			else
				_buttons[_selectedCol]->unselect();
			_selectedRow = (_selectedRow + 1) % 4;
			_moveCooldown = ROW_COOLDOWN;
			if (_selectedRow < (int)_sliders.size())
				_sliders[_selectedRow]->setTexturePosition(glm::vec2(PANEL_SIZE.x, 192.f + CONTROLS_SPRITESHEET_SIZE.y) / TEXTURE_SIZE);
			else
				_buttons[_selectedCol]->select();
		}
		else if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT)) {
			//Calculate if we are going right or left
			float offset = 0.f - (float)(Game::instance().getSpecialKey(GLUT_KEY_LEFT)) + (float)(Game::instance().getSpecialKey(GLUT_KEY_RIGHT));
			//Special treatment for volume controls. Default changes highlighted button
			switch (_selectedRow) {
				case row::MASTER:
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT)) _sliders[0]->moveLeft();
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT)) _sliders[0]->moveRight();

					_masterOffset = _sliders[0]->updateValueFromPos();
					SoundManager::instance().setMasterVolume(_masterOffset);
					break;
				case row::MUSIC:
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT)) _sliders[1]->moveLeft();
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT)) _sliders[1]->moveRight();
					_musicOffset = _sliders[0]->updateValueFromPos();
					SoundManager::instance().setMusicVolume(_musicOffset);
					break;
				case row::SOUND:
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT)) _sliders[2]->moveLeft();
					if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT)) _sliders[2]->moveRight();
					_soundOffset = _sliders[0]->updateValueFromPos();
					SoundManager::instance().setSoundVolumeFactor(_soundOffset);
					break;
				default:
					_buttons[_selectedCol]->unselect();
					_selectedCol = (_selectedCol + (int)offset + _buttons.size())%_buttons.size();
					_buttons[_selectedCol]->select();
					break;
			}
		}
	}


	//Enter
	if (Game::instance().getKeyJustPressed(13) && _selectedRow == row::BUTTONS){

		_buttons[_selectedCol]->use();
	}

	if (Game::instance().getKeyReleased(27)) {
		confirm();
	}
}

void Pause::toMenu()
{
	_action = action::QUIT;
	_state = state::FADE_OUT;
}

void Pause::confirm()
{
	_action = action::CONTINUE;
	_state = state::FADE_OUT;
	_fadeTime = 0;
}

void Pause::cancel()
{
	//Set initial volumes
	SoundManager::instance().setMasterVolume(_masterVol);
	SoundManager::instance().setMusicVolume(_musicVol);
	SoundManager::instance().setSoundVolumeFactor(_soundVol);

	_action = action::CONTINUE;
	_state = state::FADE_OUT;
	_fadeTime = 0;
}
