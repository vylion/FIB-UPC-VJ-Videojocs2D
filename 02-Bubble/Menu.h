#pragma once

#include "..\Scene.h"

class Menu
{
public:
	Menu();
	void init();
	void update(int deltaTime);

	void render();

	bool wantsToQuit();

private:
	int _selectedButton;
	Texture *_b_Texture;
	Sprite *_b_play, *_b_options, *_b_exit;
	bool _playing, _wantsToQuit;
	Scene _scene;
	ShaderProgram _texProgram;

	void initShaders();
	void play();
	void openOptions();
	void exit();
};