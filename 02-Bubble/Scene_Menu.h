#pragma once

#include "..\Scene.h"

class Scene_Menu :
	public Scene
{
public:

	Scene_Menu() {}

	void init() override;
	int update(int deltaTime) override;
	void render() override;

	int getLevelToOpen() override;

private:
	//Selection and change button cooldown
	int _selectedButton, _moveCooldown;
	int _fadeTime;

	//Textures
	Texture *_b_Texture, *_bg_Texture;

	//Buttons
	Sprite *_b_play, *_b_options, *_b_exit;
	vector<Sprite*> _buttons;
	//Logo
	Sprite *_logo;
	//Background
	Sprite *_bg;

	void initAudio() override;
	void checkButtons(int deltaTime) override;
};