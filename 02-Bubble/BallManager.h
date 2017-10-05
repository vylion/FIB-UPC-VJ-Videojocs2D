#ifndef _BALLMANAGER_INCLUDE
#define _BALLMANAGER_INCLUDE

#include <vector>
#include <string>
using namespace std;

//#include "Ball.h"

class BallManager
{
public:
	BallManager();
	~BallManager();

	void init(const string &levelFile);
	void update(int deltaTime);
	void render() const;

	
	bool ballUpdatesLeft();
	/*
	Ball *getNextHeldBall();
	void launchHeldBall(Ball *heldBall, float angle);
	*/


private:
	//Ball *_ballMatrix, *_heldBall, *_nextBall;

};
#endif