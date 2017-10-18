#include "Scene_Level.h"

#include "../Game.h"

#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 26

Scene_Level::Scene_Level() : Scene()
{
	map = NULL;
	aimer = NULL;
	bmng = NULL;
}

Scene_Level::~Scene_Level()
{
	if (map != NULL)
		delete map;
	if (aimer != NULL)
		delete aimer;
	if (bmng != NULL)
		delete aimer;
}

void Scene_Level::init(int level)
{

	_level = level;
	initShaders();
	

	string levelFiller = "";
	if (_level < 10) {
		levelFiller = "0";
	}
	string levelLocation = "../levels/level" + levelFiller + std::to_string(_level);
	map = TileMap::createTileMap(levelLocation + "_Tile.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	bmng = BallManager::createBallManager(levelLocation + "_Ball.txt", glm::ivec2(SCREEN_X + 16, SCREEN_Y + 16), map->getMapSize(), texProgram);

	aimer = new Aimer();
	aimer->init(glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES), texProgram, bmng);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene_Level::update(int deltaTime)
{
	currentTime += deltaTime;

	aimer->update(deltaTime);
	bmng->update(deltaTime);
}

void Scene_Level::render()
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
}

int Scene_Level::getState()
{
	return 0;
}

int Scene_Level::getLevel()
{
	return _level;
}
