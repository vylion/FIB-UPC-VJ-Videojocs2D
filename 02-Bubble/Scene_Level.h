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

	int getLevelToOpen() override;

private:
	enum levelStatus {RUNNING, WON, LOST, PAUSED};
	//Current level
	int _level, _levelStatus;
	//Tile map
	TileMap *map;
	//Aimer arrow
	Aimer *aimer;
	//Ball manager
	BallManager *bmng;
	
	Texture *_spriteTexture;
	Sprite *_levelNumber;
	Sprite *_b_quit, *_b_retry, *_b_nextLevel;

	void initAudio() override;
	void checkButtons(int deltaTime) override;
};

