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
				glm::vec2 _minBallCoords,
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
	vector<vector<Ball_InMatrix*> > _ballMatrix;
	vector<vector<bool> > _connectedMatrix;
	int _visibleMatrixHeight;

	glm::vec2  _ballSizeInSpritesheet, _matrixOffset;
	int _ballSize;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;

	Ball_InMatrix * ballFromColor(int &color);
	void passRowToShown();

	Ball_InMatrix::posT snapToGrid(Ball *b);
	list<Ball_InMatrix::posT> checkNeighbors(const Ball_InMatrix::posT &b);
};

#pragma once