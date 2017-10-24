#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "../Sprite.h"

class Ball
{
public:
	//sizeinspritesheet, spritesheet, shaderprogram
	Ball(const int &size, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram & shaderProgram);

	void init(int color, const glm::vec2 &position, glm::vec2 minRenderCoords);
	void update(int &deltaTime);
	void render();

	int getColor();
	void setColor(int color);

	int getSize();
	void setSize(int &size);

	glm::vec2 getSpritesheetSize();
	Texture * getTexture();

	glm::vec2 getPosition();
	void setPosition(const glm::vec2 &pos);

	vector<glm::vec2> collisionPoints();

private:
	glm::vec2 _position, _sizeInSpritesheet, _minRenderCoords;
	int _size, _color;
	Sprite *_sprite;
	Texture *_texture;
};
#endif

