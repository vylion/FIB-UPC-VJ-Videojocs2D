#include "Scene_Credits.h"
#include "../Game.h"

#define CREDITS_TEXTURE "../media/images/credits_sprites.png"
#define CREDITS_SIZE glm::vec2(260.f, 275.f)

#define FADE_TIME 1000


void Scene_Credits::init()
{

	initShaders();
	_state = state::RUNNING;
	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(CREDITS_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load howtoplay texture");


	creditsHeight = SCREEN_HEIGHT;

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);
	_credits = Sprite::createSprite(CREDITS_SIZE, CREDITS_SIZE, _bg_tex, &texProgram);
		_credits->setPosition(glm::vec2((SCREEN_WIDTH - CREDITS_SIZE.x)/2.f, SCREEN_HEIGHT));
		_credits->setTexturePosition(glm::vec2(640.f/900.f, 0.f));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0;
}

int Scene_Credits::update(int deltaTime)
{
	currentTime += deltaTime;

	creditsHeight = SCREEN_HEIGHT - currentTime / 10.f;
	
	_credits->setPosition(glm::vec2((SCREEN_WIDTH - CREDITS_SIZE.x)/2, creditsHeight));

	//Check keypresses for back to menu
	if (Game::instance().getKeyJustPressed(32) || Game::instance().getKeyReleased(27) ||		//Space, Esc
		Game::instance().getKeyJustPressed(13) || Game::instance().getKeyJustPressed('q') ||	//Enter, q
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_DOWN) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_UP)
		)
		_state = state::OPEN_LEVEL;

	//When info is completely off-screen, return to main menu
	if (currentTime/10.f >= SCREEN_HEIGHT + CREDITS_SIZE.y) _state = state::OPEN_LEVEL;

	return _state;
}

void Scene_Credits::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	float alpha = 1.0f;
	if (currentTime <= FADE_TIME)
		alpha = min(1.0f, (float)currentTime / (float)FADE_TIME);
	else if (creditsHeight <= -CREDITS_SIZE.y/2.f)
		alpha = min(1.0f, (CREDITS_SIZE.y + creditsHeight) / (CREDITS_SIZE.y/2.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, alpha);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();
	_credits->render();
}
