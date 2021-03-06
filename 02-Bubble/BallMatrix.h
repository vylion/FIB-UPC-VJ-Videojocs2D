#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include "Ball_InMatrix.h"
#include "Ball_Falling.h"

class BallMatrix
{
public:
	enum State {
		RUNNING, UPDATING, WON, LOST
	};

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

	State update(int &deltaTime);

	bool shakeAnim;
	int shakeCount, shakeSide;
	glm::vec2 shake;
	int descendAnimLeft;
	void render();

	bool checkCollision(Ball * b);
	bool addBallToMat(Ball_InMatrix::posT ballPos, unsigned int color);

	int ballsLeft();
	unsigned int colorsLeftInMatrix();
	void lowerRowsOnNextBall();

private:
	typedef Ball_InMatrix::posT posT;
	const static int DESCEND_ANIM_TIME = 1000;
	const static int SHAKE_COUNT_MAX = 30;

	std::vector< std::vector<Ball_InMatrix*> > _ballMatrix;
	std::vector< std::vector<bool> > _connectedMatrix;
	int _visibleMatrixHeight;
	std::vector<posT> _collisionFrontier;
	std::vector<Ball_Falling *> fallingBalls;

	glm::vec2  _ballSizeInSpritesheet, _minBallCoords, _minRenderCoords;
	int _ballSize;
	Texture *_spritesheet;
	ShaderProgram _shaderProgram;

	Ball_InMatrix * ballFromColor(int &color);
	void passRowToShown();

	posT snapToGrid(Ball *b, posT pos);
	std::vector<posT> checkBallsAround(const Ball_InMatrix::posT &b);
	std::vector<Ball_InMatrix::posT> checkSpaceAround(const posT & b);
	void checkPopping(const posT & b, const unsigned int & color, std::vector<posT>& pop);
	std::vector<posT> checkNotHanging();
	bool popNotHanging();
	bool popBall(posT & p);
	bool inMatrixRange(const Ball_InMatrix::posT &pos);
	bool validBall(const Ball_InMatrix::posT &pos);
	bool validSpace(const posT & pos);
	void updateFrontier();
};

#pragma once