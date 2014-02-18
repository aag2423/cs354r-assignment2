/*
-----------------------------------------------------------------------------
Filename:    Ball.cpp
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#include "Ball.h"
#include <iostream>

Ball::Ball(Ogre::SceneManager* mSceneMgr, PhysicsEngine& engine, PlayGround* box, const Ogre::Vector3& pos) :  
	parentNode(0),
	physicsEngine(&engine),
	colliding(false),
	shooting(false)
{
	Ogre::Entity* entBall = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	entBall->setCastShadows(true);
	entBall->setMaterialName("Examples/TennisBall");

	parentNode = box->getNode()->createChildSceneNode(pos);
	parentNode->attachObject(entBall);

	Ogre::Real radius = entBall->getBoundingRadius()* (1.0f - Ogre::MeshManager::getSingleton().getBoundsPaddingFactor());
	Ogre::Real ratio = BALL_RADIUS/radius;
	parentNode->scale(ratio, ratio, ratio);
	
	physicsObject.setToSphere(
		BALL_RADIUS, 
		2, 
		btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),
		btVector3(pos.x,pos.y,pos.z) 
	);
	physicsObject.setRestitution(0.99);
	physicsObject.setLinearVelocity(btVector3(0,0,0));
	physicsObject.setFriction(0.5);  
	physicsObject.setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));
	physicsObject.toggleRigidBodyAndKinematic(); // change to Kinematic
	engine.addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Ball::~Ball(void) {
	std::cout << "========= Debug: Ball Deleted =========" << std::endl;
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

bool Ball::hitBy(bool hitting, Player* player, Ogre::Vector3 shotDirection) {
	if (!hitting) {
		shooting =false;
		return false;
	}
	bool playSound = false;
	Ogre::SceneNode* playerNode = player->getNode();
	Ogre::Vector3 dir = playerNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 distance = parentNode->getPosition() - (playerNode->getPosition() + Ogre::Vector3(0, 40, 0));
	if(distance.length() < 150 && distance.dotProduct(dir) >= 0) {
		//physicsObject.setLinearVelocity(btVector3(0, 30, -100));
		dir *= 100;
		//physicsObject.setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
		if (shotDirection.y == 0) shotDirection.y = 10;
		physicsObject.setLinearVelocity(btVector3(shotDirection.x, shotDirection.y, -100));
		if (!shooting)
			playSound = true;
		shooting = true;
	} else
		shooting =false;
	return playSound;
}


BallCollisionEvent Ball::collidesWith(PlayGround* court, Player* p) {
	MyPlaneContactResultCallback callback;
	BallCollisionEvent result = NOTHING_HAPPENED;
        btDiscreteDynamicsWorld* world = physicsEngine->getPhysicsWorld();
	btRigidBody* me = physicsObject.getRigidBody();

	world->contactPairTest(me, p->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_PLAYER;
	} else {
	world->contactPairTest(me, court->getPhysicsObject(TOP_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_CEILING;
	}
	world->contactPairTest(me, court->getPhysicsObject(LEFT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
	}
	world->contactPairTest(me, court->getPhysicsObject(RIGHT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
	}
	world->contactPairTest(me, court->getPhysicsObject(FRONT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
	}
	world->contactPairTest(me, court->getPhysicsObject(BACK_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_TARGET;
	}
	world->contactPairTest(me, court->getPhysicsObject(BOTTOM_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_FLOOR;
	}
	}
	if (colliding) {
		if (result == NOTHING_HAPPENED)
			colliding = false;
		return NOTHING_HAPPENED;
	} else {
		if (result != NOTHING_HAPPENED) colliding = true;
		return result;
	}
}
//-------------------------------------------------------------------------------------
void Ball::setPositionAndPause(const Ogre::Vector3& pos) {
	parentNode->setPosition(pos);
	physicsEngine->removeObject(&physicsObject);
	physicsObject.toggleRigidBodyAndKinematic();
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	physicsObject.setWorldTransform(trans);
	physicsEngine->addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

void Ball::resume(void) {
	physicsEngine->removeObject(&physicsObject);
	physicsObject.toggleRigidBodyAndKinematic(2);
	physicsEngine->addObject(&physicsObject);
}
