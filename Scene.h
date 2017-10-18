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
//#include "02-Bubble/Scene_Menu.h"

class Scene
{

public:
	Scene() {}

	virtual void init() {};
	virtual void init(int level) {};

	virtual void update(int deltaTime) {};
	virtual void render() {};

protected:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	void initShaders();

};
#endif // _SCENE_INCLUDE

