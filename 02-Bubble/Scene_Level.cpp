#include "Scene_Level.h"

#include "../Game.h"

#define MIN_SCREEN_X 32
#define MIN_SCREEN_Y 16

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
	_state = RUNNING;
	_level = level;
	initShaders();
	

	string levelFiller = "";
	if (_level < 10) {
		levelFiller = "0";
	}
	string levelLocation = "../levels/level" + levelFiller + std::to_string(_level);

	/*----------------------------------------TILEMAP-------------------------------------------------------*/

	map = TileMap::createTileMap(levelLocation + "_Tile.txt", glm::vec2(MIN_SCREEN_X, MIN_SCREEN_Y), texProgram);


	/*----------------------------------------BALLMANAGER---------------------------------------------------*/

	bmng = BallManager::createBallManager(levelLocation + "_Ball.txt", map, texProgram);
	bmng->init();
	//bmng->init(glm::ivec2(MIN_SCREEN_X + map->getBallOffset().x, MIN_SCREEN_Y + map->getBallOffset().y));

	/*----------------------------------------AIMER-------------------------------------------------------*/

	aimer = new Aimer();
	//Horizontal position is ball starting position + ball space / 2
	//Vertical position is ball starting position + ball space - 2 to compensate for aimer height
	glm::vec2 aimerDist = map->getBallSpace() * glm::vec2(0.5f, 1.f) + glm::vec2(0.5f, -2);
	glm::vec2 aimerPos = map->getBallOffset();
	aimer->init(aimerPos + aimerDist, texProgram, bmng);

	/*----------------------------------------END-------------------------------------------------------*/

	//SoundManager::instance().setMusic(MUSIC_FILE);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

int Scene_Level::update(int deltaTime)
{
	currentTime += deltaTime;

	aimer->update(deltaTime);
	bmng->update(deltaTime);

	if (!bmng->ballsLeft()) {
		//Overlay to ask if next level or straight to menu(?)
		//Could also be used to display statistics(?)
			//^ We need a text class for that
		_state = OPEN_LEVEL;
		_level = -1; //Menu
	}

	if (Game::instance().getKeyReleased(27)) {
		_level = -2;
		return OPEN_LEVEL;
	}

	return _state;
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

int Scene_Level::getLevelToOpen()
{
	return _level + 1;
}
