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

class Scene
{

public:
	enum state { RUNNING, OPEN_LEVEL, EXIT };

	Scene() {}

	virtual void init() {};
	virtual void init(int level) {};

	virtual int update(int deltaTime) { return OPEN_LEVEL; };
	virtual void render() {};

	virtual int getLevelToOpen() { return -1; };

protected:
	int _state;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	void initShaders();

};
#endif // _SCENE_INCLUDE

