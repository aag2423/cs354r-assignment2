/*
-----------------------------------------------------------------------------
Filename:    Ball.h
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#ifndef __Ball_h_
#define __Ball_h_


#include <Ogre.h>
#include "PlayGround.h"

class Ball {
protected:
	const static Ogre::Real BALL_RADIUS = 10.0f;
	static Ogre::Real ballSpeed;
	Ogre::SceneNode* parentNode;
	Ogre::Real ballMoveRange;
	Ogre::Vector3 ballDirection;
	bool checkCollision(Ogre::Vector3& direction, Ogre::Vector3& from);
public:
	Ball(Ogre::SceneManager* mSceneMgr, PlayGround& box, Ogre::Real x, Ogre::Real y, Ogre::Real z);
	~Ball(void);
	void move(const Ogre::FrameEvent& evt);
	bool bounceWall(void);
	bool collideWith(Ball* otherBall);
	static Ogre::Real getSpeed(void) { return Ball::ballSpeed; }
	static void setSpeed(Ogre::Real speed) {  if(speed > 9 && speed < 251) Ball::ballSpeed = speed;}
	Ogre::Vector3& getDirection() { return ballDirection; }
	Ogre::SceneNode* getNode() { return parentNode; }
	
};
#endif // #ifndef __Ball_h_
