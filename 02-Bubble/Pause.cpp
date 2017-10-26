#include <algorithm>

#include "Pause.h"
#include "../Game.h"
#include "SoundManager.h"
#include "Button.h"

#include <GL/glut.h>

#define FADE_TIME 100.f
#define ROW_COOLDOWN 50
#define TEXTURE_FILE "../../media/images/pause_sprites.png"


#define PANEL_SIZE glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)
#define PANEL_SPRITESHEET_SIZE PANEL_SIZE

#define CONTROLS_SIZE PANEL_SIZE*glm::vec2(0.8f,1.f/6.f)
#define CONTROLS_SPRITESHEET_SIZE CONTROLS_SIZE

#define BUTTON_SIZE glm::vec2(10.f, 10.f)
#define BUTTON_PIXEL_SIZE BUTTON_SIZE


void Pause::init(bool inMenu, ShaderProgram &shaderProgram)
{
	_shaderProgram = shaderProgram;
	_texture = Texture();
	if (!_texture.loadFromFile(TEXTURE_FILE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load pause texture");

	/*----------------------------------------MUSIC-------------------------------------------------------*/

	SoundManager::instance().pauseMusic(true);
	_masterVol = SoundManager::instance().getMasterVolume();
	_masterOffset = _masterVol;
	_musicVol = SoundManager::instance().getMusicVolume();
	_musicOffset = _musicVol;
	_soundVol = SoundManager::instance().getSoundVolumeFactor();
	_soundOffset = _soundVol;

	/*----------------------------------------BUTTONS-------------------------------------------------------*/
	_selectedRow = 3;
	_selectedCol = 1 + inMenu; 

	/*----------------------------------------BACKGROUND----------------------------------------------------*/

	/*----------------------------------------SLIDERS-------------------------------------------------------*/

	_action = action::WAIT;
	_state = state::FADE_IN;
}

int Pause::update(int deltaTime)
{
	_fadeTime += (float)deltaTime;

	switch (_state) {
		case state::RUNNING:
			checkButtons(deltaTime);
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
				_state = state::RUNNING;
			}
			break;
	}
	return _action;
}

void Pause::render()
{

}

void Pause::checkButtons(int deltaTime)
{
	_moveCooldown = max(_moveCooldown - deltaTime, 0);

	//No keys pressed
	if (_moveCooldown != 0.f && 
		(Game::instance().getSpecialKeyReleased(GLUT_KEY_UP)    ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_DOWN)  ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKeyReleased(GLUT_KEY_LEFT))
		)
		_moveCooldown = 0;
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			_selectedRow = (_selectedRow + 3) % 4;
			_moveCooldown = ROW_COOLDOWN;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			_selectedRow = (_selectedRow + 1) % 4;
			_moveCooldown = ROW_COOLDOWN;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			//Calculate if we are going right or left
			float offset = 0 - (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) + (Game::instance().getSpecialKey(GLUT_KEY_LEFT));
			//Special treatment for volume controls. Default changes highlighted button
			switch (_selectedRow) {
				case row::MASTER:
					_masterOffset = min(1.0f, max(0.f, (_masterOffset + offset*0.1f)));
					SoundManager::instance().setMasterVolume(_masterOffset);
					break;
				case row::MUSIC:
					_musicOffset = min(1.0f, max(0.f, (_musicOffset + offset*0.1f)));
					SoundManager::instance().setMasterVolume(_musicOffset);
					break;
				case row::SOUND:
					_soundOffset = min(1.0f, max(0.f, (_soundOffset + offset*0.1f)));
					SoundManager::instance().setMasterVolume(_soundOffset);
					break;
				default:
					//_buttons[_selectedCol]->setTexturePosition(glm::vec2(, _selectedCol*));
					_selectedCol = (_selectedCol + (int)offset + _buttons.size())%_buttons.size();
					break;
			}
		}
	}
	//Enter
	if (Game::instance().getKey(32))
		switch (_selectedRow) {
			case row::BUTTONS:
				switch (_selectedCol) {
					case 0:
						quit();
						break;
					case 1:
						cancel();
						break;
					case 2:
						confirm();
						break;
				}
				break;
		}
}

void Pause::quit()
{
	_action = action::QUIT;
	_state = state::FADE_OUT;
}

void Pause::confirm()
{
	_state = state::FADE_OUT;
}

void Pause::cancel()
{
	//Set initial volumes
	SoundManager::instance().setMasterVolume(_masterVol);
	SoundManager::instance().setMusicVolume(_musicVol);
	SoundManager::instance().setSoundVolumeFactor(_soundVol);

	_state = state::FADE_OUT;
}
