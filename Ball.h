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
#include "PhysicsEngine.h"

class Ball {
protected:
	const static Ogre::Real BALL_RADIUS = 5.0f;
	Ogre::SceneNode* parentNode;
	PhysicsObject physicsObject;
public:
	Ball(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround& box, Ogre::Real x, Ogre::Real y, Ogre::Real z);
	~Ball(void);
	Ogre::SceneNode* getNode() { return parentNode; }
	void hitBy(Ogre::SceneNode* player);
	void updateGraphicsScene(void);
	PhysicsObject& getPhysicsObject(void) { return physicsObject; }
};
#endif // #ifndef __Ball_h_
