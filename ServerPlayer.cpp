#include "ServerPlayer.h"
#include <iostream>

ServerPlayer::ServerPlayer(Ogre::SceneManager* mSceneMgr, PhysicsEngine& bulletEngine, PlayerType type, const Ogre::Vector3& pos, PlayerSide side, bool turnAround)
{	
	playerType = type;
	playerSide = side;
	graphicsEngine = mSceneMgr;
	positionNode = graphicsEngine->getRootSceneNode()->createChildSceneNode(pos);
	if (turnAround) positionNode->yaw(Ogre::Degree(180));
	physicsEngine = &bulletEngine;
	physicsObject.setToBox(
		btVector3(10,HEIGHT,1),
		100,
		btQuaternion(0, 0, 0, 1),
		btVector3(pos.x, pos.y, pos.z)
	);
	physicsObject.toggleRigidBodyAndKinematic(); // change to Kinematic

	physicsObject.setRestitution(0.001); 
	physicsObject.getRigidBody()->setAngularFactor(0);
	physicsObject.setFriction(1);  
	
	physicsEngine->addObject(&physicsObject);


	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
	playerState.step = NORMAL_STEP;
	playerState.shotDirection = 0;
	playerState.yaw = 0;
		playerState.serverPlayerSide = playerSide;


	if (type == AI) {
		playerState.step = SMALL_STEP;
		playerState.hitting = true;
		playerState.shotDirection = Ogre::Vector3(0, 30, 0);
		playerState.strength = NORMAL_HIT;
	}
}

//-------------------------------------------------------------------------------------

ServerPlayer::~ServerPlayer(void) {
	physicsEngine->removeObject(&physicsObject);
	graphicsEngine->getRootSceneNode()->removeChild(positionNode);
	std::cout << "========= Debug: ServerPlayer Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ServerPlayer::setPosition(const Ogre::Vector3& newPos) {
	positionNode->setPosition(newPos);
	btTransform playerTrans;
	physicsObject.getWorldTransform(playerTrans);
	playerTrans.setOrigin(btVector3(newPos.x, newPos.y, newPos.z));
	physicsObject.setWorldTransform(playerTrans);
}

//-------------------------------------------------------------------------------------

bool ServerPlayer::move(const Ogre::Vector3& range, bool isFullGame) {
	Ogre::Vector3 direction(0, 0, 0);
	if (playerState.movingLeft)
		direction.x -= playerState.step;
	if (playerState.movingRight)
		direction.x += playerState.step;
	if (playerState.movingForward)
		direction.z -= playerState.step;
	if (playerState.movingBackward)
		direction.z += playerState.step;

	direction *= 1.0/120.0;
	positionNode->translate(direction, Ogre::Node::TS_LOCAL);
	Ogre::Vector3 playerPos = positionNode->getPosition();
	if (playerType != AI){
		positionNode->yaw(Ogre::Degree(playerState.yaw), Ogre::Node::TS_LOCAL);
	}


	if(playerPos.x < -range.x) playerPos.x = -range.x;
	else if(playerPos.x > range.x) playerPos.x = range.x;
	if (isFullGame){
		if (playerSide == SIDE_NEAR) {
			if (playerPos.z < 20) playerPos.z = 20;
			else if (playerPos.z > range.z) playerPos.z = range.z;
		} else {
			if (playerPos.z > -20) playerPos.z = -20;
			else if (playerPos.z < -range.z) playerPos.z = -range.z;
		}
	} else {
		if(playerPos.z < -range.z) playerPos.z = -range.z;
		else if(playerPos.z > range.z) playerPos.z = range.z;
	}

	setPosition(playerPos);
	return direction.x !=0 || direction.z !=0;
}

//-------------------------------------------------------------------------------------

bool ServerPlayer::canReachBall(const Ogre::Vector3& ballPos) {
	Ogre::Vector3 distance = ballPos - (positionNode->getPosition() + Ogre::Vector3(0, HEIGHT*0.5, 0));

	Ogre::Vector3 dir = positionNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	if (playerType == AI) {
		return distance.length() < 50 && distance.dotProduct(dir) >= 0;
	}
	else 
		return distance.length() < PADDLE_REACH && distance.dotProduct(dir) >= 0;
}
