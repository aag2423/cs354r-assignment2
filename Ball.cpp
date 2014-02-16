/*
-----------------------------------------------------------------------------
Filename:    Ball.cpp
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#include "Ball.h"

Ogre::Real Ball::ballSpeed = 120;

Ball::Ball(Ogre::SceneManager* mSceneMgr, PlayGround& box, Ogre::Real x, Ogre::Real y, Ogre::Real z) :  
	parentNode(0),
	ballMoveRange(0),
	ballDirection(Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom(), Ogre::Math::UnitRandom())
{
	Ogre::Entity* entBall = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	entBall->setCastShadows(true);
	entBall->setMaterialName("Examples/TennisBall");

	parentNode = box.getNode()->createChildSceneNode(Ogre::Vector3(x, y, z));
	parentNode->attachObject(entBall);

	Ogre::Real radius = entBall->getBoundingRadius();
	Ogre::Real ratio = BALL_RADIUS/radius;
	parentNode->scale(ratio, ratio, ratio);

	ballMoveRange = box.getHalfSideLength() - BALL_RADIUS;
	ballDirection.normalise();
}
//-------------------------------------------------------------------------------------

Ball::~Ball(void) {
}
//-------------------------------------------------------------------------------------
void Ball::move(const Ogre::FrameEvent& evt) {
	parentNode->pitch( Ogre::Degree(-ballDirection.y/2));
	parentNode->yaw( Ogre::Degree(ballDirection.z/2));
	parentNode->roll( Ogre::Degree(-ballDirection.x/2));
	parentNode->translate(ballSpeed * evt.timeSinceLastFrame * ballDirection);
}
//-------------------------------------------------------------------------------------
bool Ball::bounceWall(void) {
	bool bounce = false;
	Ogre::Vector3 ballPosition = parentNode->getPosition();
	if(ballPosition.x>ballMoveRange && ballDirection.x>0.0f) {ballDirection.x = -ballDirection.x; bounce = true;}
	if(ballPosition.x<-ballMoveRange && ballDirection.x<0.0f) {ballDirection.x = -ballDirection.x; bounce = true;}
	if(ballPosition.y>ballMoveRange && ballDirection.y>0.0f) {ballDirection.y = -ballDirection.y; bounce = true;}
	if(ballPosition.y<-ballMoveRange && ballDirection.y<0.0f) {ballDirection.y = -ballDirection.y; bounce = true;}
	if(ballPosition.z>ballMoveRange && ballDirection.z>0.0f) {ballDirection.z = -ballDirection.z; bounce = true;}
	if(ballPosition.z<-ballMoveRange && ballDirection.z<0.0f) {ballDirection.z = -ballDirection.z; bounce = true;}
	return bounce;
}
//-------------------------------------------------------------------------------------

bool Ball::checkCollision(Ogre::Vector3& direction, Ogre::Vector3& from) {
	if(direction.x * from.x > 0 && direction.y * from.y > 0 && direction.z * from.z > 0)
		return false;
	else
		return true;
}

//--------------------------------------------------------------------------

bool Ball::collideWith(Ball* otherBall) {
	bool collided = true;
	Ogre::SceneNode* otherNode = otherBall->getNode();
	Ogre::Vector3 ballPosition = parentNode->getPosition();
	Ogre::Vector3 ballPosition2 = otherNode->getPosition();
	Ogre::Vector3 fromOther(
		ballPosition.x-ballPosition2.x, 
		ballPosition.y-ballPosition2.y, 
		ballPosition.z-ballPosition2.z
	);
	if(fromOther.length() <= BALL_RADIUS*2) {
		fromOther.normalise();
		if (checkCollision(ballDirection, fromOther))
			ballDirection = ballDirection.reflect(fromOther);
		else
			collided = false;
		Ogre::Vector3 fromThis = fromOther*(-1);
		if (checkCollision(otherBall->ballDirection, fromThis))
			otherBall->ballDirection = otherBall->ballDirection.reflect(fromThis);
		else
			collided = false;
	} else
			collided = false;
	return collided;
}

