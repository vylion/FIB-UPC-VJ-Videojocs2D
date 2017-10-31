#pragma once
#include "../Scene.h"
class Scene_Credits :
	public Scene
{
public:
	Scene_Credits() {};

	void init() override;

	int update(int deltaTime) override;
	void render() override;

	int getLevelToOpen() override { return -1; };
private:
	Sprite *_credits;
	float creditsHeight;
};

