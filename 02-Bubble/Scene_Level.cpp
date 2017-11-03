#include "Scene_Level.h"

#include "../Game.h"

#define MIN_SCREEN_COORDS glm::vec2(32.f, 16.f)

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 26

#define BACKGROUND_TEXTURE "../media/images/level_bg.png"
#define SPRITE_TEXTURE "../media/images/level_sprites.png"

#define LEVEL_NUMBER_SIZE glm::vec2(24.f,24.f)
#define LEVEL_NUMBER_POSITION glm::vec2(552.f, 52.f)
#define LEVEL_NUMBER_SPRITESHEET_SIZE glm::vec2(16.f, 16.f)
#define LEVEL_NUMBER_SPRITESHEET_POSITION glm::vec2(208.f, 0.f)

#define SCORE_SIZE glm::vec2(96.f, 24.f)
#define SCORE_POSITION glm::vec2(480.f, 144.f)
#define SCORE_SPRITESHEET_POSITION  LEVEL_NUMBER_SPRITESHEET_POSITION


#define PANEL_SIZE glm::vec2(256.f,128.f)
#define PANEL_POSITION (glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) - PANEL_SIZE)/2.f
#define PANEL_SPRITESHEET_POSITION glm::vec2(0.f, 128.f)

#define YOU_SIZE glm::vec2(87.f, 28.f)
#define YOU_SPRITESHEET_POSITION glm::vec2(0.f, 64.f)
#define YOU_POSITION PANEL_POSITION + glm::vec2(25.f,20.f)
#define WON_SIZE glm::vec2(93.f, 27.f)
#define WON_SPRITESHEET_POSITION glm::vec2(0.f, 96.f)
#define LOST_SIZE glm::vec2(105.f, 27.f)
#define LOST_SPRITESHEET_POSITION glm::vec2(96.f, 96.f)
#define WONLOST_ENDPOSITION PANEL_POSITION + glm::vec2(231.f, 20.f)

#define CLEAR_SIZE glm::vec2(160.f, 32.f)
#define CLEAR_SPRITESHEET_POSITION glm::vec2(96.f, 64.f)
#define CLEAR_POSITION PANEL_POSITION + glm::vec2(48.f, 16.f)

#define BUTTON_SIZE glm::vec2(64.f, 32.f)
#define LEFT_BUTTON_POS PANEL_POSITION + glm::vec2(48.f, 72.f)
#define RIGHT_BUTTON_POS PANEL_POSITION + glm::vec2(144.f, 72.f)
#define BUTTON_SPRITESHEET_SIZE glm::vec2(64.f, 32.f)
#define BUTTON_SPRITESHEET_POSITION glm::vec2(16.f, 0.f)

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

	/*----------------------------------------TEXTURES-----------------------------------------------------*/

	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(BACKGROUND_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load level BG");

	_spriteTexture = new Texture();
	if (!_spriteTexture->loadFromFile(SPRITE_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load level sprites");


	/*----------------------------------------TILEMAP------------------------------------------------------*/

	map = TileMap::createTileMap(levelLocation + "_Tile.txt", MIN_SCREEN_COORDS, texProgram);


	/*----------------------------------------BALLMANAGER--------------------------------------------------*/

	bmng = BallManager::createBallManager(map, texProgram);
	bmng->init(levelLocation + "_Ball.txt");
	//bmng->init(glm::ivec2(MIN_SCREEN_X + map->getBallOffset().x, MIN_SCREEN_Y + map->getBallOffset().y));

	/*----------------------------------------AIMER--------------------------------------------------------*/

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

	_scoreVal = 9999;

	_score1 = Sprite::createSprite(LEVEL_NUMBER_SIZE, LEVEL_NUMBER_SPRITESHEET_SIZE, _spriteTexture, &texProgram);
	_score10 = Sprite::createSprite(LEVEL_NUMBER_SIZE, LEVEL_NUMBER_SPRITESHEET_SIZE, _spriteTexture, &texProgram);
	_score100 = Sprite::createSprite(LEVEL_NUMBER_SIZE, LEVEL_NUMBER_SPRITESHEET_SIZE, _spriteTexture, &texProgram);
	_score1000 = Sprite::createSprite(LEVEL_NUMBER_SIZE, LEVEL_NUMBER_SPRITESHEET_SIZE, _spriteTexture, &texProgram);

	_score.push_back(_score1);
	_score.push_back(_score10);
	_score.push_back(_score100);
	_score.push_back(_score1000);

	for (unsigned int i = 0; i < _score.size(); i++)
		_score[i]->setPosition(SCORE_POSITION + glm::vec2(_score.size()-i-1, 0.f)*SCORE_SIZE/(float)_score.size());
	updateScoreSprites();

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);
	
	/*----------------------------------------PANEL--------------------------------------------------------*/

	_panel = Sprite::createSprite(PANEL_SIZE, PANEL_SIZE, _spriteTexture, &texProgram);
		_panel->setPosition(PANEL_POSITION);
		_panel->setTexturePosition(PANEL_SPRITESHEET_POSITION / _spriteTexture->getSize());
	_levelClearSprite = Sprite::createSprite(CLEAR_SIZE, CLEAR_SIZE, _spriteTexture, &texProgram);
		_levelClearSprite->setPosition(CLEAR_POSITION);
		_levelClearSprite->setTexturePosition(CLEAR_SPRITESHEET_POSITION / _spriteTexture->getSize());

	_youSprite = Sprite::createSprite(YOU_SIZE, YOU_SIZE, _spriteTexture, &texProgram);
		_youSprite->setPosition(YOU_POSITION);
		_youSprite->setTexturePosition(YOU_SPRITESHEET_POSITION / _spriteTexture->getSize());
	_wonSprite = Sprite::createSprite(WON_SIZE, WON_SIZE, _spriteTexture, &texProgram);
		_wonSprite->setPosition(WONLOST_ENDPOSITION - WON_SIZE * glm::vec2(1.f, 0.f));
		_wonSprite->setTexturePosition(WON_SPRITESHEET_POSITION / _spriteTexture->getSize());
	_lostSprite = Sprite::createSprite(LOST_SIZE, LOST_SIZE, _spriteTexture, &texProgram);
		_lostSprite->setPosition(WONLOST_ENDPOSITION - LOST_SIZE * glm::vec2(1.f, 0.f));
		_lostSprite->setTexturePosition(LOST_SPRITESHEET_POSITION / _spriteTexture->getSize());

	_b_quit = new Button(BUTTON_SIZE, BUTTON_SIZE, _spriteTexture, &texProgram);
		_b_quit->init(LEFT_BUTTON_POS, (BUTTON_SPRITESHEET_POSITION + BUTTON_SIZE * glm::vec2(0.f,0.f)) / _spriteTexture->getSize());
		_b_quit->setCallback([this](void) { quit(); });
	_b_retry = new Button(BUTTON_SIZE, BUTTON_SIZE, _spriteTexture, &texProgram);
		_b_retry->init(RIGHT_BUTTON_POS, (BUTTON_SPRITESHEET_POSITION + BUTTON_SIZE * glm::vec2(1.f, 0.f)) / _spriteTexture->getSize());
		_b_retry->setCallback([this](void) { retry(); });
	_b_nextLevel = new Button(BUTTON_SIZE, BUTTON_SIZE, _spriteTexture, &texProgram);
		_b_nextLevel->init(RIGHT_BUTTON_POS, (BUTTON_SPRITESHEET_POSITION + BUTTON_SIZE * glm::vec2(2.f, 0.f)) / _spriteTexture->getSize());
		_b_nextLevel->setCallback([this](void) { next(); });

	_buttons.push_back(_b_quit);
	_buttons.push_back(_b_retry);
	_buttons.push_back(_b_nextLevel);
	/*----------------------------------------END----------------------------------------------------------*/

	//SoundManager::instance().setMusic(MUSIC_FILE);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

int Scene_Level::update(int deltaTime)
{
	currentTime += deltaTime;
	checkButtons(deltaTime);

	switch (_levelStatus) {
		//Not won or lost, keep updating sprites
		case levelStatus::RUNNING:
			switch (_state) {
				//Not paused, keep updating game elements
				case state::RUNNING:
					{
						int bmngState = bmng->update(deltaTime);
						switch (bmngState) {
							//No balls left in BallManager
						case BallManager::state::WON:
							win();
							break;
							//Balls reached bottom
						case BallManager::state::LOST:
							lose();
							break;
						}
						aimer->update(deltaTime, bmngState);
						if (currentTime > 2000 && _level == 1) {
							win();
							//lose();
						}
					}
					break;
				//Paused, stop updating game elements and update pause panel
				case state::PAUSED:
					//Give control to pause menu
					switch (Pause::instance().update(deltaTime)) {
						case Pause::CONTINUE:
							_state = RUNNING;
							break;
						case Pause::QUIT:
							quit();
							break;
					}
					break;
			}
			break;
	}
	return _state;
}

void Scene_Level::render()
{

	float alpha = 1.0f;
	//Dim background elements if there's a panel in front
	if (_levelStatus != levelStatus::RUNNING || _state == state::PAUSED) alpha = 0.6f;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//BG image
	_bg->render();
	//Tiles
	map->render();
	//Aiming arrow and held ball
	aimer->render();
	//Level number on top right area
	_levelNumber->render();
	for (unsigned int i = 0; i < _score.size(); i++) _score[i]->render();
	//Different balls
	bmng->render();

	//Reset alpha to draw foreground elements
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	switch (_levelStatus) {
		case levelStatus::WON:
			//Render win panel
			_panel->render();
			//Display "YOU WON!" on the last level
			if (_level == 5) {
				_youSprite->render();
				_wonSprite->render();
			}
			//Otherwise display "Level cleared!" and button to enter next game
			else {
				_levelClearSprite->render();
				_b_nextLevel->render();
			}
			//Always display quit button
			_b_quit->render();
			break;
		case levelStatus::LOST:
			//Render loss panel
			_panel->render();
			//Display "YOU LOST" regardless of level
			_youSprite->render();
			_lostSprite->render();
			//Always display quit button
			_b_quit->render();
			//Display retry button
			_b_retry->render();
			break;
		default:
			//Pause render
			if (_state == state::PAUSED) Pause::instance().render();
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

	//Press enter on a button while on results panel
	if (_levelStatus != levelStatus::RUNNING){

		_moveCooldown = max(0, _moveCooldown - deltaTime);
		if (Game::instance().getKeyJustPressed(13) || Game::instance().getKeyJustPressed(32)) {

			_buttons[_selectedButton]->use();
		}
		else if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT) && _moveCooldown == 0) {
			_moveCooldown = 50;
			_buttons[_selectedButton]->unselect();
			_selectedButton = 1 + (_levelStatus == levelStatus::WON);
			_buttons[_selectedButton]->select();
		}
		else if (Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) && _moveCooldown == 0) {
			_moveCooldown = 50;
			_buttons[_selectedButton]->unselect();
			_selectedButton = 0;
			_buttons[_selectedButton]->select();
		}
	}

	if (Game::instance().getKeyReleased(27)) {
		_state = state::PAUSED;
	}
}

void Scene_Level::updateScoreSprites()
{
	for (unsigned int i = 0; i < _score.size(); ++i) {
		//1, 10, 100, 1000 respectively
		int this_pow = int(pow(10, i));
		//10, 100, 1000, 10000 respectively
		int next_pow = int(pow(10, i + 1));
		//Get the number until the i-th digit, then select the i-th digit
		int digitNum = (_scoreVal % next_pow) / this_pow;

		//Select texture position based on digit
		_score[i]->setTexturePosition((SCORE_SPRITESHEET_POSITION + glm::vec2(digitNum%3,digitNum/3)*16.f) / _spriteTexture->getSize());
	}
}

void Scene_Level::win()
{
	//Set to pause for the alpha rendering
	_state = state::PAUSED;
	//Indicate we win, not loss
	_levelStatus = levelStatus::WON;
	//Default selection to next level
	_moveCooldown = 0;
	_selectedButton = 2;
	bool playSound = false;
	_buttons[_selectedButton]->select(playSound);
}

void Scene_Level::lose()
{
	//Set to pause for the alpha rendering
	_state = state::PAUSED;
	//Indicate we win, not loss
	_levelStatus = levelStatus::LOST;
	//Default selection to next level
	_moveCooldown = 0;
	_selectedButton = 1;
	bool playSound = false;
	_buttons[_selectedButton]->select(playSound);
}

void Scene_Level::quit()
{
	_levelStatus = levelStatus::RUNNING;
	_state = state::OPEN_LEVEL;
	_level = -2; //-1 or any negative int opens menu
}

void Scene_Level::retry()
{
	_levelStatus = levelStatus::RUNNING;
	_state = state::OPEN_LEVEL;
	_level -= 1;
}

void Scene_Level::next()
{
	_levelStatus = levelStatus::RUNNING;
	_state = OPEN_LEVEL;
}
