#include "Player.h"
#include <iostream>

Player::Player(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos) :  
	parentNode(0),
	court(box)
{
	playerEnt = mSceneMgr->createEntity( "playerEnt", "ninja.mesh" );
	playerEnt->setCastShadows(true);
	parentNode = box->getNode()->createChildSceneNode(pos);
	parentNode->attachObject(playerEnt);


	walkAnimation = playerEnt->getAnimationState("Walk");
        walkAnimation->setLoop(true);
        walkAnimation->setEnabled(true);

	shootAnimation = playerEnt->getAnimationState("Attack3");
        shootAnimation->setLoop(true);
        shootAnimation->setEnabled(true);
	/*
	Ogre::Entity* paddleEnt = mSceneMgr->createEntity( "paddleEnt", "cube.mesh" );
	paddleEnt->setCastShadows(true);
	paddleNode = parentNode->createChildSceneNode(Ogre::Vector3(pos.x, pos.y + 300.0, pos.z - 250));
	paddleNode->scale(0.5, 0.5, 0.1);
	paddleNode->attachObject(paddleEnt);
	*/

	Ogre::Real height = playerEnt->getBoundingBox().getSize().y;
	Ogre::Real ratio = HEIGHT/height;
	parentNode->scale(ratio, ratio, ratio);

	Ogre::Vector3 collsionBox = playerEnt->getBoundingBox().getSize();

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
	std::cout << "========= Debug: Player Deleted =========" << std::endl;
}

void Player::move(PlayerState& playerState, const Ogre::FrameEvent& evt) {
	
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
	if(playerPos.z < -range.z) playerPos.z = -range.z;
	else if(playerPos.z > range.z) playerPos.z = range.z;
	parentNode->setPosition(playerPos);
	
	Ogre::Quaternion q = parentNode->getOrientation();

	if(direction.x != 0 || direction.z != 0)
		if (playerState.step == NORMAL_STEP)
			walkAnimation->addTime(evt.timeSinceLastFrame*4);
		else		
			walkAnimation->addTime(evt.timeSinceLastFrame*8);
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
