#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 26


Scene::Scene()
{
	map = NULL;
	//player = NULL;
	aimer = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(aimer != NULL)
		delete aimer;
	/*
	if(player != NULL)
		delete player;
	*/
}


void Scene::init(ShaderProgram _texProgram, int level)
{
	//initShaders();
	texProgram = _texProgram;

	string levelFiller = "";
	if (level < 10) {
		levelFiller = "0";
	}
	string levelLocation = "../levels/level" + levelFiller + std::to_string(level);
	map = TileMap::createTileMap(levelLocation + "_Tile.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	
	//bmng = new BallManager();
	bmng = BallManager::createBallManager(levelLocation + "_Ball.txt", glm::ivec2(SCREEN_X+16, SCREEN_Y+16), map->getMapSize(), texProgram);

	aimer = new Aimer();
	aimer->init(glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), texProgram, bmng);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	aimer->update(deltaTime);
	bmng->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	aimer->render();
	bmng->render();
	//player->render();
}
