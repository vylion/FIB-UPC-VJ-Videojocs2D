#pragma once

#include "..\Scene.h"
#include "Button.h"

class Scene_Menu :
	public Scene
{
public:

	Scene_Menu() {}
	~Scene_Menu() {
		SoundManager::instance().dropAll();
	}
		
	void init() override;
	int update(int deltaTime) override;
	void render() override;

	int getLevelToOpen() override;

private:
	//Selection and change button cooldown
	int _selectedButton, _halfButtonSide, _moveCooldown;
	int _fadeTime;

	//Textures
	Texture *_b_Texture;

	//Buttons
	Button *_b_play, *_b_howtoplay, *_b_credits, *_b_options, *_b_exit;
	vector<Button*> _buttons;
	//Logo
	Sprite *_logo;

	void initAudio() override;
	void checkButtons(int deltaTime) override;

	void play();
	void howtoplay();
	void credits();
	void options();
	void quit();
};