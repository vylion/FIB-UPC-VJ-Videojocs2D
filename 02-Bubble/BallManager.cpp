#include "BallManager.h"



BallManager::BallManager()
{
}


BallManager::~BallManager()
{
}

void BallManager::init(const string & levelFile)
{
	_heldBall = getNewBall();
	_nextBall = getNewBall();
}

void BallManager::update(int deltaTime)
{
	_heldBall->update(deltaTime);
}

void BallManager::render() const
{
}

bool BallManager::ballUpdatesLeft()
{
	return false;
}

Ball * BallManager::getNextHeldBall()
{
	
	//Store current held ball for the return
	Ball *ret = _heldBall;
	//Make current held ball the stored ball
	_heldBall = _nextBall;
	//Generate a new stored ball
	_nextBall = getNewBall();
	//Return the previously held ball
	return ret;
}

void BallManager::launchHeldBall(Ball * heldBall, float angle)
{
	_launchedBall = heldBall;
	_launchedBall->launch(angle);
}

Ball * BallManager::getNewBall()
{
	return nullptr;
}
