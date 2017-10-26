#pragma once

#include <vector>

#include "../Sprite.h"

class Pause
{
public:
	static enum action {WAIT, CONTINUE, QUIT};
	Pause() {};

	static Pause instance() {
		static Pause P;
		return P;
	}

	void init(bool inMenu, ShaderProgram &shaderProgram);
	int update(int deltaTime);
	void render();
	
private:
	static enum state {RUNNING, FADE_IN, FADE_OUT};
	static enum row {MASTER, MUSIC, SOUND, BUTTONS};
	int _state, _action;

	//Time spent in paused since changing to FADE_* state
	float _fadeTime;
	//Alpha and position ratio
	float _ratio;

	int _selectedRow, _selectedCol, _moveCooldown;
	//Text + slider
	Sprite *_masterVolumeSprite, *_musicVolumeSprite, *_soundVolumeSprite;
	//Slider marker
	Sprite *_masterVolumeMarker, *_musicVolumeMarker, *_soundVolumeMarker;
	//Buttons
	Sprite *_b_quit, *_b_cancel, *_b_confirm;
	vector<Sprite*> _buttons;
	//Background
	Sprite *_bg, *_alphaSprite;

	float _masterOffset, _musicOffset, _soundOffset;
	float _masterVol, _musicVol, _soundVol;

	Texture _texture;
	ShaderProgram _shaderProgram;

	void checkButtons(int deltaTime);
	void quit();
	void confirm();
	void cancel();
};

