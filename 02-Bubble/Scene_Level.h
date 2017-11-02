#pragma once
#include "../Scene.h"

class Scene_Level :
	public Scene
{
public:
	Scene_Level();
	~Scene_Level();

	void init(int level) override;
	int update(int deltaTime) override;
	void render() override;

	//Defaults to _level + 1
	int getLevelToOpen() override;

private:
	enum levelStatus {RUNNING, WON, LOST};
	//Current level
	int _level, _levelStatus, _scoreVal, _moveCooldown, _selectedButton;
	//Tile map
	TileMap *map;
	//Aimer arrow
	Aimer *aimer;
	//Ball manager
	BallManager *bmng;
	
	Texture *_spriteTexture;
	//Current level to be displayed on top right area
	Sprite *_levelNumber;
	//Score sprites
	Sprite *_score1000, *_score100, *_score10, *_score1;
	vector<Sprite*> _score;
	//Result panel buttons
	Button *_b_quit, *_b_retry, *_b_nextLevel;
	vector<Button*> _buttons;
	//Result panel sprites
	Sprite *_panel, *_levelClearSprite, *_youSprite, *_wonSprite, *_lostSprite;

	void initAudio() override;
	void checkButtons(int deltaTime) override;

	void updateScoreSprites();

	void win();
	void lose();

	//Back to menu
	void quit();
	//Restart level
	void retry();
	//Next level
	void next();
};

