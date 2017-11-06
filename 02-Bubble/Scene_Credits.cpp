#include "Scene_Credits.h"
#include "../Game.h"

#define CREDITS_BG_TEXTURE "media/images/credits_bg.png"
#define CREDITS_TEXTURE "media/images/credits_sprites.png"

#define CREDITS_MUSIC "media/audio/music/credits_bgm.ogg"

#define FADE_TIME 1000
#define WAIT_TIME FADE_TIME + 1000


void Scene_Credits::init()
{

	initShaders();
	initAudio();
	_state = state::RUNNING;
	_fadeouttime = 0;

	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(CREDITS_BG_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load credits bg texture");
	_creditsSpritesheet = new Texture();
	if (!_creditsSpritesheet->loadFromFile(CREDITS_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load credits texture");

	creditsHeight = 0;

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);
	_credits = Sprite::createSprite(_creditsSpritesheet->getSize(), _creditsSpritesheet->getSize(), _creditsSpritesheet, &texProgram);
		_credits->setPosition(glm::vec2((SCREEN_WIDTH - _creditsSpritesheet->getSize().x)/2.f, 0.f));
		//_credits->setTexturePosition(glm::vec2(640.f/900.f, 0.f));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0;
}

int Scene_Credits::update(int deltaTime)
{
	currentTime += deltaTime;
	

	if (currentTime >= WAIT_TIME) {
		creditsHeight = (WAIT_TIME - currentTime) / 10.f;
		if (creditsHeight <= -_creditsSpritesheet->getSize().y) {
			_fadeouttime += deltaTime;
			SoundManager::instance().setMusicVolume(max(0,FADE_TIME - _fadeouttime) / (float)FADE_TIME);
		}
	}
	else if (currentTime <= FADE_TIME) {
		SoundManager::instance().setMusicVolume(min(currentTime, (float)FADE_TIME) / (float)FADE_TIME);
	}
	
	_credits->setPosition(glm::vec2((SCREEN_WIDTH - _creditsSpritesheet->getSize().x)/2, creditsHeight));

	//Check keypresses for back to menu
	if (Game::instance().getKeyJustPressed(32) || Game::instance().getKeyReleased(27) ||		//Space, Esc
		Game::instance().getKeyJustPressed(13) || Game::instance().getKeyJustPressed('q') ||	//Enter, q
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_DOWN) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_UP)
		)
		_state = state::OPEN_LEVEL;

	//When info is completely off-screen, return to main menu
	if (_fadeouttime >= FADE_TIME) _state = state::OPEN_LEVEL;

	return _state;
}

void Scene_Credits::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	float alpha = 1.0f;
	
	if (creditsHeight <= -_creditsSpritesheet->getSize().y) {
		alpha = 1.f - min(1.f, (float)_fadeouttime / (float)FADE_TIME);
	}
	else if (currentTime <= FADE_TIME)
		alpha = min(1.f, (float)currentTime / (float)FADE_TIME);

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();
	_credits->render();
}

void Scene_Credits::initAudio()
{
	SoundManager::instance().setMusic(CREDITS_MUSIC);
	SoundManager::instance().setMusicVolume(0.f);
}
