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
	Texture *_creditsSpritesheet;
	Sprite *_credits;

	int _fadeouttime;

	float creditsHeight;
};

