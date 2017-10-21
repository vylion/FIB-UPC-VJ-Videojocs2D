#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	scene = new Scene_Menu();
	scene->init();
}

bool Game::update(int deltaTime)
{
	switch (scene->update(deltaTime)) {
		case Scene::OPEN_LEVEL:
			if (scene->getLevelToOpen() < 0) {
				scene = new Scene_Menu();
				scene->init();
			}
			else {
				int level = scene->getLevelToOpen();
				scene = new Scene_Level();
				scene->init(level);
			}

			break;

		case Scene::EXIT:
			bPlay = false;
			break;
	}
	for (int i = 0; i < 256; i++) {
		keys_released[i] = false;
		specialKeys_released[i] = false;
	}
	return bPlay;
}


void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->render();
}

void Game::keyPressed(int key)
{
	//if(key == 27) // Escape code
	//	bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	keys_released[key] = true;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	specialKeys_released[key] = true;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getKeyReleased(int key) const
{
	return keys_released[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::getSpecialKeyReleased(int key) const
{
	return specialKeys_released[key];
}





