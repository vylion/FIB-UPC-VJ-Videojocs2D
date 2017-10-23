#pragma once

#include <vector>
#include <algorithm>
#include "Ball_InMatrix.h"

class BallMatrix
{
public:
	BallMatrix() {}
	BallMatrix(	int *_colorMatrix,
				glm::ivec2 &matrixDimensions,
				int visibleMatrixHeight,
				glm::vec2 _ballOffset,
				const int &ballSize,
				const glm::vec2 &ballSizeInSpritesheet,
				Texture *spritesheet,
				ShaderProgram &shaderProgram);

	void update(int &deltaTime);
	void render();

	bool checkCollision(Ball * b);
	void addBallToMat(Ball * b);

	int ballsLeft();

private:
	std::vector< std::vector<Ball_InMatrix*> > _ballMatrix;
	std::vector< std::vector<bool> > _connectedMatrix;
	int _visibleMatrixHeight;

	glm::vec2  _ballSizeInSpritesheet, _matrixOffset;
	int _ballSize;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;

	Ball_InMatrix * ballFromColor(int &color);
	void passRowToShown();

	Ball_InMatrix::posT snapToGrid(Ball *b);
	std::vector<Ball_InMatrix::posT> checkBallsAround(const Ball_InMatrix::posT &b);
	std::list<Ball_InMatrix::posT> popNeighbors(const Ball_InMatrix::posT &b);
	bool inMatrix(const Ball_InMatrix::posT &pos);
};

#pragma once