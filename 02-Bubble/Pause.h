#pragma once

#include <vector>

#include "../Sprite.h"
#include "Button.h"

class Pause
{
public:
	enum action {WAIT, CONTINUE, QUIT};
	Pause() {};

	static Pause &instance() {
		static Pause P;
		return P;
	}

	void init(ShaderProgram *shaderProgram);
	void clearAll();
	int update(int deltaTime);
	void render();
	
private:
	enum state {RUNNING, FADE_IN, FADE_OUT, RETURNED};
	enum row {MASTER, MUSIC, SOUND, BUTTONS};
	int _state, _action;

	//Time spent in paused since changing to FADE_* state
	float _fadeTime;
	//Alpha and position ratio
	float _ratio;
	//
	bool dragging;

	int _selectedRow, _selectedCol, _moveCooldown;
	//Slider marker
	Sprite *_masterVolumeMarker, *_musicVolumeMarker, *_soundVolumeMarker;
	vector<Sprite*> _sliders;
	//Buttons
	Button *_b_quit, *_b_cancel, *_b_confirm;
	vector<Button*> _buttons;
	//Background
	Sprite *_panel, *_alphaSprite;

	float _masterOffset, _musicOffset, _soundOffset;
	float _masterVol, _musicVol, _soundVol;

	glm::mat4 projection;
	Texture *_texture;
	ShaderProgram *_shaderProgram;

	void setSliderPositions();
	void checkMouse();

	void checkButtons(int deltaTime);
	void toMenu();
	void confirm();
	void cancel();
};