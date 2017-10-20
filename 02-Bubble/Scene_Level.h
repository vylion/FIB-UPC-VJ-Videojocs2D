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
	//Current level
	int _level;
	//Tile map
	TileMap *map;
	//Aimer arrow
	Aimer *aimer;
	//Ball manager
	BallManager *bmng;
};

