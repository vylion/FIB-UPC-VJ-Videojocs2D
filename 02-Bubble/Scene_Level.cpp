#include "Scene_Level.h"

#include "../Game.h"

#define MIN_SCREEN_COORDS glm::vec2(32.f, 16.f)

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
	//initAudio();

	string levelFiller = "";
	if (_level < 10) {
		levelFiller = "0";
	}
	string levelLocation = "../levels/level" + levelFiller + std::to_string(_level);

	/*----------------------------------------TILEMAP-------------------------------------------------------*/

	map = TileMap::createTileMap(levelLocation + "_Tile.txt", MIN_SCREEN_COORDS, texProgram);


	/*----------------------------------------BALLMANAGER---------------------------------------------------*/

	bmng = BallManager::createBallManager(map, texProgram);
	bmng->init(levelLocation + "_Ball.txt");
	//bmng->init(glm::ivec2(MIN_SCREEN_X + map->getBallOffset().x, MIN_SCREEN_Y + map->getBallOffset().y));

	/*----------------------------------------AIMER-------------------------------------------------------*/

	aimer = new Aimer();
	//Horizontal position is ball starting position + ball space
	//Vertical position is ball starting position + ball space
	glm::vec2 aimerDist = (glm::vec2)map->getBallSpace() * glm::vec2(0.5f, 1.f);
	glm::vec2 aimerPos = (glm::vec2)map->getBallOffset();
	glm::vec2 pos = (aimerPos + aimerDist) * 16.f;
	aimer->init(pos, MIN_SCREEN_COORDS, texProgram, bmng);

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

	checkButtons(deltaTime);

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

void Scene_Level::initAudio()
{
	//SoundManager::instance().setMusic(MUSIC_FILE);

	//SoundManager::instance().addSound(CHANGE_BUTTON_SFX);
	//SoundManager::instance().setSoundVolume(CHANGE_BUTTON_SFX, 0.1f);
}

void Scene_Level::checkButtons(int deltaTime)
{
	//M stands for music
	if (Game::instance().getKeyJustPressed('m')) {
		SoundManager::instance().toggleMusicPause();
	}

	//Escape to menu. Open menu in the future?
	if (Game::instance().getKeyReleased(27)) {
		_level = -2;
		_state = OPEN_LEVEL;
	}
}
