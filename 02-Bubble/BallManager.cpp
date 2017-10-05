#include "BallManager.h"



BallManager::BallManager()
{
}


BallManager::~BallManager()
{
}

void BallManager::init(const string & levelFile)
{
}

void BallManager::update(int deltaTime)
{
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
	return nullptr;
}

void BallManager::launchHeldBall(Ball * heldBall, float angle)
{
}
