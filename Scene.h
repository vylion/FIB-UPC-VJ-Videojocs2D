#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"
#include "TileMap.h"

#include "02-Bubble/Aimer.h"
#include "02-Bubble/BallManager.h"
#include "02-Bubble/SoundManager.h"

class Scene
{

public:
	enum state { RUNNING, FADE_IN, FADE_OUT, OPEN_LEVEL, HOW_TO_PLAY, OPEN_CREDITS, PAUSED, EXIT };

	Scene() {}

	virtual void init() {};
	virtual void init(int level) {};

	virtual int update(int deltaTime) { return OPEN_LEVEL; };
	virtual void render() {};

	virtual int getLevelToOpen() { return -1; };

protected:
	int _state;
	
	Texture *_bg_tex;
	Sprite *_bg;


	ShaderProgram texProgram;
	float currentTime, pauseTime;
	glm::mat4 projection;

	void initShaders();
	void initPause();
	virtual void initAudio() {};
	virtual void checkButtons(int deltaTime) {};
	void renderPause();

};
#endif // _SCENE_INCLUDE

