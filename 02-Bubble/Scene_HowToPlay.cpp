#include "Scene_HowToPlay.h"
#include "../Game.h"

#define HOWTOPLAY_TEXTURE "../media/images/howtoplay.png"

void Scene_HowToPlay::init()
{

	initShaders();
	_state = state::RUNNING;
	_bg_tex = new Texture();
	if (!_bg_tex->loadFromFile(HOWTOPLAY_TEXTURE, TEXTURE_PIXEL_FORMAT_RGBA)) printf("Failed to load howtoplay texture");

	_bg = Sprite::createSprite(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), _bg_tex, &texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
}

int Scene_HowToPlay::update(int deltaTime)
{
	//Check back to menu
	if (Game::instance().getKeyJustPressed(32) || Game::instance().getKeyReleased(27) ||		//Space, Esc
		Game::instance().getKeyJustPressed(13) || Game::instance().getKeyJustPressed('q') ||	//Enter, q
		Game::instance().mouseJustPressed() ||
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_LEFT) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKeyJustPressed(GLUT_KEY_DOWN) || Game::instance().getSpecialKeyJustPressed(GLUT_KEY_UP)
		)
		return state::OPEN_LEVEL;
	return _state;
}

void Scene_HowToPlay::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	_bg->render();
}
