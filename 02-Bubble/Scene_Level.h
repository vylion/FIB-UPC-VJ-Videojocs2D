#pragma once
#include "../Scene.h"

class Scene_Level :
	public Scene
{
public:
	Scene_Level();
	~Scene_Level();

	void init(int level) override;
	void update(int deltaTime) override;
	void render() override;

private:
	TileMap *map;
	Aimer *aimer;
	BallManager *bmng;
};

