#pragma once

#include "..\Scene.h"

class Scene_Menu :
	public Scene
{
public:
	enum state { NONE, PLAY, OPTIONS, EXIT };

	Scene_Menu() {}

	void init() override;
	void update(int deltaTime);
	void render() override;

	int getState() override;
	int getLevel() override;

private:
	int _selectedButton, _moveCooldown, _state;

	//Textures
	Texture *_b_Texture, *_bg_Texture;

	//Buttons
	Sprite *_b_play, *_b_options, *_b_exit;
	vector<Sprite*> _buttons;
	//Logo
	Sprite *_logo;
	//Background
	Sprite *_bg;

	//State switch
	void play();
	void openOptions();
	void exit();
};