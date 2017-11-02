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
				int levelHeight,
				glm::vec2 minBallCoords,
				glm::vec2 minRenderCoords,
				const int &ballSize,
				const glm::vec2 &ballSizeInSpritesheet,
				Texture *spritesheet,
				ShaderProgram &shaderProgram);

	void update(int &deltaTime);
	void render();

	bool checkCollision(Ball * b);
	void addBallToMat(Ball * b, Ball_InMatrix::NeighborBalls pos);

	int ballsLeft();
	unsigned int colorsLeftInMatrix();

private:
	typedef Ball_InMatrix::posT posT;

	std::vector< std::vector<Ball_InMatrix*> > _ballMatrix;
	std::vector< std::vector<bool> > _connectedMatrix;
	int _visibleMatrixHeight;

	glm::vec2  _ballSizeInSpritesheet, _minBallCoords, _minRenderCoords;
	int _ballSize;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;

	bool comparePosT(const Ball_InMatrix::posT& a, const Ball_InMatrix::posT& b);
	Ball_InMatrix * ballFromColor(int &color);
	void passRowToShown();

	Ball_InMatrix::posT snapToGrid(Ball *b);
	std::vector<posT> checkBallsAround(const Ball_InMatrix::posT &b);
	std::list<posT> popNeighbors(const Ball_InMatrix::posT &b);
	bool inMatrix(const Ball_InMatrix::posT &pos);
};

#pragma once