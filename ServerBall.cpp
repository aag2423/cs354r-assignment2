#include "ServerBall.h"
#include <iostream>

ServerBall::ServerBall(PhysicsEngine& engine, const Ogre::Vector3& pos) : 
	physicsEngine(engine),
	colliding(false),
	shooting(false)
{	
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
	physicsEngine.addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

ServerBall::~ServerBall(void) {
	physicsEngine.removeObject(&physicsObject);
	std::cout << "========= Debug: ServerBall Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

bool ServerBall::hitByPlayer(ServerPlayer* player) {
	InputPlayerState* playerState = &(player->playerState);
	if (!playerState->hitting) return false;
	
	PlayerSide side = playerState->serverPlayerSide;

	btTransform trans;
	physicsObject.getWorldTransform(trans);
	btVector3 ballPos = trans.getOrigin();
	if (side == SIDE_NEAR && ballPos.getZ() < 0) return false;
	if (side == SIDE_FAR && ballPos.getZ() > 0) return false;
	if (!player->canReachBall(Ogre::Vector3(ballPos.getX(),ballPos.getY(),ballPos.getZ())))
		return false;

	Ogre::Vector3 shotDirection = playerState->shotDirection;
	if (shotDirection.y == 0) shotDirection.y = 20;
	Ogre::Real z = playerState->strength;
	std::cout << shotDirection.x << std::endl;
	std::cout << shotDirection.y << std::endl;
	if (side == SIDE_NEAR ) {z = -z;}
	physicsObject.setLinearVelocity(btVector3(shotDirection.x, shotDirection.y, z));
	return true;
}

BallCollisionEvent ServerBall::collidesWithHelper(BallCollisionEvent result) {
	if (colliding) {
		if (result == NOTHING_HAPPENED)
			colliding = false;
		return NOTHING_HAPPENED;
	} else {
		if (result != NOTHING_HAPPENED) colliding = true;
		return result;
	}
}

BallCollisionEvent ServerBall::checkCollision(ServerCourt* court, ServerPlayer* p1, ServerPlayer* p2) {
	MyPlaneContactResultCallback callback;
	BallCollisionEvent result = NOTHING_HAPPENED;
        btDiscreteDynamicsWorld* world = physicsEngine.getPhysicsWorld();
	btRigidBody* me = physicsObject.getRigidBody();

	world->contactPairTest(me, p1->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_PLAYER;
		return collidesWithHelper(result);
	}
	world->contactPairTest(me, p2->getPhysicsObject().getRigidBody(), callback);
	if (callback.hit) {	
		result = HIT_OPPONENT;
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

const Ogre::Vector3& ServerBall::getPosition(void) {
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	btVector3 ballPos = trans.getOrigin();
	return *(new Ogre::Vector3(ballPos.getX(),ballPos.getY(),ballPos.getZ()));
}

const Ogre::Quaternion& ServerBall::getOrientation(void) {
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	btQuaternion ballOrient = trans.getRotation();
	return *(new Ogre::Quaternion(ballOrient.w(),ballOrient.x(),ballOrient.y(),ballOrient.z()));
}
