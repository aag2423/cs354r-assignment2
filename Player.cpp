#include "Player.h"
#include <iostream>

Player::Player(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayerType type, PlayGround* box, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr),
	parentNode(0),
	visible(true),
	engine(&physicsEngine),
	court(box)
{
	playerState.type = type;
	resetState();


	parentNode = box->getNode()->createChildSceneNode(pos);
	Ogre::String paddleName;
	if (type == HUMAN) {
		playerEnt = mSceneMgr->createEntity( "playerEnt", "ninja.mesh" );
		walkAnimation = playerEnt->getAnimationState("Walk");
		shootAnimation = playerEnt->getAnimationState("Attack3");
		playerNode = parentNode->createChildSceneNode();
		playerNode->attachObject(playerEnt);
		Ogre::Real ratio = HEIGHT/195.649;
		playerNode->scale(ratio, ratio, ratio);
		paddleName = "playerPaddle";
	} else {
		playerEnt = mSceneMgr->createEntity( "computerEnt", "robot.mesh" );
		walkAnimation = playerEnt->getAnimationState("Walk");
		shootAnimation = playerEnt->getAnimationState("Shoot");
		playerNode = parentNode->createChildSceneNode();
		playerNode->attachObject(playerEnt);
		playerNode->yaw(Ogre::Degree(90));
		Ogre::Real ratio = HEIGHT/101.673;
		playerNode->scale(ratio, ratio, ratio);
		paddleName = "CPUPaddle";
	}
	playerEnt->setCastShadows(true);

        walkAnimation->setLoop(true);
        walkAnimation->setEnabled(true);

        shootAnimation->setLoop(true);
        shootAnimation->setEnabled(true);
	
	physicsObject.setToBox(
		btVector3(10,HEIGHT,5),
		100,
		btQuaternion(0, 0, 0, 1),
		btVector3(pos.x, pos.y, pos.z)
	);
	physicsObject.toggleRigidBodyAndKinematic(); // change to Kinematic

	physicsObject.setRestitution(0.001); 
	physicsObject.getRigidBody()->setAngularFactor(0);
	physicsObject.setFriction(1);  

	physicsEngine.addObject(&physicsObject);
}

//-------------------------------------------------------------------------------------

Player::~Player(void) {
	engine->removeObject(&physicsObject);
	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity(playerEnt);
	std::cout << "========= Debug: Player Deleted =========" << std::endl;
}

void Player::toggleVisible(void) {
	if (visible) {
		engine->removeObject(&physicsObject);
		playerNode->detachObject(playerEnt);
		visible = false;
	} else {
		engine->addObject(&physicsObject);
		playerNode->attachObject(playerEnt);
		visible = true;
	}
}

void Player::resetState(void) {
	if (playerState.type == AI) {
		playerState.strength = NORMAL_HIT;
		playerState.step = SMALL_STEP;
		playerState.hitting = true;
		playerState.shotDirection = Ogre::Vector3(0, 30, 0);
	} else {
		playerState.strength = STRONG_HIT;
		playerState.step = NORMAL_STEP;
		playerState.hitting = false;
		playerState.shotDirection = 0;
	}
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
}

void Player::move(const Ogre::FrameEvent& evt, bool isFullGame) {
	
	Ogre::Vector3 direction(0, 0, 0);
	if (playerState.movingLeft)
		direction.x -= playerState.step;
	if (playerState.movingRight)
		direction.x += playerState.step;
	if (playerState.movingForward)
		direction.z -= playerState.step;
	if (playerState.movingBackward)
		direction.z += playerState.step;
	direction *= evt.timeSinceLastFrame;

	parentNode->translate(direction, Ogre::Node::TS_LOCAL);
	Ogre::Vector3 range;
	court->getHalfMovaRange(range);

	Ogre::Vector3 playerPos = parentNode->getPosition();
	if(playerPos.x < -range.x) playerPos.x = -range.x;
	else if(playerPos.x > range.x) playerPos.x = range.x;
	if (isFullGame){
		if (playerState.type == HUMAN && playerPos.z < 10) playerPos.z = 10;
		if (playerState.type == HUMAN && playerPos.z > range.z) playerPos.z = range.z;
		if (playerState.type == AI && playerPos.z > -10) playerPos.z = -10;
	} else {
		if(playerPos.z < -range.z) playerPos.z = -range.z;
		else if(playerPos.z > range.z) playerPos.z = range.z;
	}
	parentNode->setPosition(playerPos);
	
	Ogre::Quaternion q = parentNode->getOrientation();

	if(direction.x != 0 || direction.z != 0)
		if (playerState.step == NORMAL_STEP)
			walkAnimation->addTime(evt.timeSinceLastFrame*2);
		else		
			walkAnimation->addTime(evt.timeSinceLastFrame*4);
	if (playerState.hitting) 
		shootAnimation->addTime(evt.timeSinceLastFrame*2);
	else 
		shootAnimation->setTimePosition(0);
	btTransform playerTrans;
	physicsObject.getWorldTransform(playerTrans);
	playerTrans.setOrigin(btVector3(playerPos.x, playerPos.y, playerPos.z));
	playerTrans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));
	physicsObject.setWorldTransform(playerTrans);

}

//-------------------------------------------------------------------------------------

void Player::setPosition(const Ogre::Vector3& pos) {
	parentNode->setPosition(pos);
	parentNode->resetOrientation();
	btTransform trans;
	physicsObject.getWorldTransform(trans);
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	trans.setRotation(btQuaternion(0, 0, 0, 1));
	physicsObject.setWorldTransform(trans);
}
