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
	graphicsEngine(mSceneMgr),
	parentNode(0),
	physicsEngine(&engine),
	colliding(false),
	shooting(false)
{
	entBall = mSceneMgr->createEntity(Ogre::SceneManager::PT_SPHERE);
	entBall->setCastShadows(true);
	entBall->setMaterialName("Examples/TennisBall");

	parentNode = box->getNode()->createChildSceneNode(pos);
	Ogre::SceneNode* tempNode = parentNode->createChildSceneNode();
	tempNode->attachObject(entBall);
	Ogre::Real ratio = BALL_RADIUS/100;
	tempNode->scale(ratio, ratio, ratio);
	
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
	engine.addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Ball::~Ball(void) {
	physicsEngine->removeObject(&physicsObject);
	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity("entBall");
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

bool Ball::hitBy(Player* player) {
	if (!player->playerState.hitting) {
		shooting =false;
		return false;
	}
	bool playSound = false;
	Ogre::SceneNode* playerNode = player->getNode();
	Ogre::Vector3 dir = playerNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	Ogre::Vector3 distance = parentNode->getPosition() - (playerNode->getPosition() + Ogre::Vector3(0, 40, 0));
	Ogre::Real limit = player->playerState.type==AI ? 50 : 80;
	if(distance.length() < limit && distance.dotProduct(dir) >= 0) {
		//physicsObject.setLinearVelocity(btVector3(0, 30, -100));
		dir *= 100;
		//physicsObject.setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
		Ogre::Vector3 shotDirection = player->playerState.shotDirection;
		if (shotDirection.y == 0) shotDirection.y = 20;
		Ogre::Real z = player->playerState.strength;
		if (player->playerState.type == HUMAN) {z = -z; }
			
		physicsObject.setLinearVelocity(btVector3(shotDirection.x, shotDirection.y, z));
		if (!shooting)
			playSound = true;
		shooting = true;
	}
	return playSound;
}


BallCollisionEvent Ball::hitTarget(Target* t1, Target* t2, Target* t3) {
	MyPlaneContactResultCallback callback;
        btDiscreteDynamicsWorld* world = physicsEngine->getPhysicsWorld();
	btRigidBody* me = physicsObject.getRigidBody();

	world->contactPairTest(me, t1->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		return HIT_TARGET_1;
	}
	world->contactPairTest(me, t2->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		return HIT_TARGET_2;
	}
	world->contactPairTest(me, t3->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		return HIT_TARGET_3;
	}
	return NOTHING_HAPPENED;
}

BallCollisionEvent Ball::collidesWithHelper(BallCollisionEvent result) {
	if (colliding) {
		if (result == NOTHING_HAPPENED)
			colliding = false;
		return NOTHING_HAPPENED;
	} else {
		if (result != NOTHING_HAPPENED) colliding = true;
		return result;
	}
}

BallCollisionEvent Ball::collidesWith(PlayGround* court, Player* p) {
	MyPlaneContactResultCallback callback;
	BallCollisionEvent result = NOTHING_HAPPENED;
        btDiscreteDynamicsWorld* world = physicsEngine->getPhysicsWorld();
	btRigidBody* me = physicsObject.getRigidBody();

	world->contactPairTest(me, p->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_PLAYER;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(TOP_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_CEILING;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(BOTTOM_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_FLOOR;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(LEFT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(RIGHT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(FRONT_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, court->getPhysicsObject(BACK_PLANE).getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_WALL;
		return collidesWithHelper(result);
	}
	return collidesWithHelper(result);
	
}
