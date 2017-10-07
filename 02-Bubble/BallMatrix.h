#ifndef _BALLMATRIX_INCLUDE
#define _BALLMATRIX_INCLUDE

#include <vector>
#include "Ball.h"

class BallMatrix
{
public:
	BallMatrix() {}
	BallMatrix(	int *_colorMatrix,
				glm::ivec2 &matrixDimensions,
				int visibleMatrixHeight,
				const glm::vec2 &ballSize,
				const glm::vec2 &ballSizeInSpritesheet,
				Texture *spritesheet,
				ShaderProgram &shaderProgram);

	void update(int &deltaTime);
	void render();

	bool checkCollision(Ball * b);
	void addBallToMat(Ball * b);

	int ballsLeft();

private:
	vector<vector<Ball*> > _shownBallMatrix;
	vector<vector<Ball*> > _hiddenBallMatrix;
	int _visibleMatrixHeight;

	glm::vec2  _ballSize, _ballSizeInSpritesheet;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;

	Ball * ballFromColor(int &color);
	void passRowToShown();
};

#endif