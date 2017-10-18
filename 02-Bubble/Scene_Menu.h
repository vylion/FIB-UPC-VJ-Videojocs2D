#pragma once

#include "..\Scene.h"

class Scene_Menu :
	public Scene
{
public:
	static enum state { NONE, PLAYING, OPTIONS, EXIT };

	Scene_Menu() {}

	void init() override;
	void update(int deltaTime);
	void render() override;

	int getState();

private:
	int _selectedButton, _moveCooldown, _state;

	Texture *_b_Texture;
	Sprite *_b_play, *_b_options, *_b_exit;
	Sprite *_bg;

	void play();
	void openOptions();
	void exit();
};