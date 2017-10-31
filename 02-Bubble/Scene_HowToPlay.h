#pragma once
#include "../Scene.h"
class Scene_HowToPlay :
	public Scene
{
public:
	Scene_HowToPlay() {};

	void init() override;

	int update(int deltaTime) override;
	void render() override;

	int getLevelToOpen() override { return -1; };
};