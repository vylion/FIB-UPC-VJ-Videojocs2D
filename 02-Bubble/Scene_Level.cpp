#include "Scene_Level.h"

#include "../Game.h"

#define MIN_SCREEN_COORDS glm::vec2(32.f, 16.f)

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 26

#define BACKGROUND_TEXTURE "../media/images/level_bg.png"
#define SPRITE_TEXTURE "../media/images/level_sprites.png"

#define LEVEL_NUMBER_SIZE glm::vec2(24.f,24.f)
#define LEVEL_NUMBER_POSITION glm::vec2(552.f, 52.f)
#define LEVEL_NUMBER_SPRITESHEET_SIZE glm::vec2(16.f,16.f)
#define LEVEL_NUMBER_SPRITESHEET_POSITION glm::vec2(208.f, 0.f)

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
	_state = state::RUNNING;
	_levelStatus = levelStatus::RUNNING;
	_level = level;
	initShaders();
	//initAudio();

	string levelFiller = "";
	if (_level < 10) {
		levelFiller = "0";
	}
	string levelLocation = "../levels/level" + levelFiller + std::to_string(_level);

	/*----------------------------------------TEXTURES-------------------------------------------------------*/

	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(BACKGROUND_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load level BG");

	_spriteTexture = new Texture();
	if (!_spriteTexture->loadFromFile(SPRITE_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load level sprites");


	/*----------------------------------------TILEMAP-------------------------------------------------------*/

	map = TileMap::createTileMap(levelLocation + "_Tile.txt", MIN_SCREEN_COORDS, texProgram);


	/*----------------------------------------BALLMANAGER---------------------------------------------------*/

	bmng = BallManager::createBallManager(map, texProgram);
	bmng->init(levelLocation + "_Ball.txt");
	//bmng->init(glm::ivec2(MIN_SCREEN_X + map->getBallOffset().x, MIN_SCREEN_Y + map->getBallOffset().y));

	/*----------------------------------------AIMER---------------------------------------------------------*/

	aimer = new Aimer();
	//Horizontal position is ball starting position + ball space
	//Vertical position is ball starting position + ball space
	glm::vec2 aimerDist = (glm::vec2)map->getBallSpace() * glm::vec2(0.5f, 1.f);
	glm::vec2 aimerPos = (glm::vec2)map->getBallOffset();
	glm::vec2 pos = (aimerPos + aimerDist) * 16.f;
	aimer->init(pos, MIN_SCREEN_COORDS, _spriteTexture, texProgram, bmng);

	/*----------------------------------------BACKGROUND---------------------------------------------------*/
	_levelNumber = Sprite::createSprite(LEVEL_NUMBER_SIZE, LEVEL_NUMBER_SPRITESHEET_SIZE, _spriteTexture, &texProgram);
		_levelNumber->setPosition(LEVEL_NUMBER_POSITION);
		_levelNumber->setTexturePosition((LEVEL_NUMBER_SPRITESHEET_POSITION + glm::vec2(_level%3,_level/3)*16.f) / _spriteTexture->getSize());
	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);
	
	/*----------------------------------------END----------------------------------------------------------*/

	//SoundManager::instance().setMusic(MUSIC_FILE);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

int Scene_Level::update(int deltaTime)
{
	currentTime += deltaTime;
	switch (_state) {
		case state::RUNNING:
			aimer->update(deltaTime);
			bmng->update(deltaTime);

			if (!bmng->ballsLeft()) {
				_state = state::PAUSED;
				_levelStatus = levelStatus::WON;
			}
			break;
		case state::PAUSED:
			break;
	}
	checkButtons(deltaTime);
	return _state;
}

void Scene_Level::render()
{

	glm::mat4 modelview;

	float alpha = 1.0f;
	if (_levelStatus != levelStatus::RUNNING) alpha = 0.6f;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();
	map->render();
	aimer->render();
	_levelNumber->render();
	bmng->render();

	switch (_levelStatus) {
		case levelStatus::WON:
			//Render win panel
			break;
		case levelStatus::LOST:
			//Render loss panel
			break;
		case levelStatus::PAUSED:
			Pause::instance().render();
			break;
	}
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
