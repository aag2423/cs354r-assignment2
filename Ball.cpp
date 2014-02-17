/*
-----------------------------------------------------------------------------
Filename:    Ball.cpp
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#include "Ball.h"

Ball::Ball(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround& box, Ogre::Real x, Ogre::Real y, Ogre::Real z) :  
	parentNode(0)
{
	Ogre::Entity* entBall = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	entBall->setCastShadows(true);
	entBall->setMaterialName("Examples/TennisBall");

	parentNode = box.getNode()->createChildSceneNode(Ogre::Vector3(x, y, z));
	parentNode->attachObject(entBall);

	Ogre::Real radius = entBall->getBoundingRadius();
	Ogre::Real ratio = BALL_RADIUS/radius;
	parentNode->scale(ratio, ratio, ratio);
	
	physicsObject.setToSphere(
		BALL_RADIUS, 
		2, 
		btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),
		btVector3(x,y,z) 
	);
	physicsObject.setRestitution(0.99);
	physicsObject.setLinearVelocity(btVector3(0,0,0));
	physicsObject.setFriction(0.5);  
	physicsObject.setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));
	physicsEngine.addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Ball::~Ball(void) {
}

//-------------------------------------------------------------------------------------

void Ball::updateGraphicsScene(void) {
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();
	btVector3 origin = trans.getOrigin();
	parentNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	parentNode->setPosition(Ogre::Vector3(origin.getX(), origin.getY(), origin.getZ()));
}

//-------------------------------------------------------------------------------------

void Ball::hitBy(Ogre::SceneNode* player) {
	btTransform trans;
	Ogre::Vector3 dir = player->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 distance = parentNode->getPosition() - player->getPosition();
	if(distance.length() < 100 && distance.dotProduct(dir) >= 0) {
		physicsObject.setLinearVelocity(btVector3(0, 30, -100));
		//dir *= 100;
		//physicsObject.setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
	}
}
