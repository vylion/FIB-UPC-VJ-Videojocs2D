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
	bool addBallToMat(Ball_InMatrix::posT ballPos, Ball_InMatrix::NeighborBalls collided, unsigned int color);

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

	Ball_InMatrix * ballFromColor(int &color);
	void passRowToShown();

	Ball_InMatrix::posT snapToGrid(Ball *b);
	std::vector<posT> checkBallsAround(const Ball_InMatrix::posT &b);
	void checkPopping(const posT & b, const unsigned int & color, std::vector<posT>& pop);
	bool popBall(posT & p);
	bool inMatrix(const Ball_InMatrix::posT &pos);
};

#pragma once