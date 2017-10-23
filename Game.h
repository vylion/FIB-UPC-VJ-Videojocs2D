#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "02-Bubble\Scene_Menu.h"
#include "02-Bubble\Scene_Level.h"



#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getKeyJustPressed(int key) const;
	bool getKeyReleased(int key) const;
	bool getSpecialKey(int key) const;
	bool getSpecialKeyJustPressed(int key);
	bool getSpecialKeyReleased(int key) const;

private:
	bool bPlay;                       // Continue to play game?
	Scene *scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	bool keys_pressed[256], specialKeys_pressed[256];
	bool keys_released[256], specialKeys_released[256];

};


#endif // _GAME_INCLUDE


