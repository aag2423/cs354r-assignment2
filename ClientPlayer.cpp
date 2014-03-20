#include "ClientPlayer.h"
#include <iostream>

ClientPlayer::ClientPlayer(Ogre::SceneManager* mSceneMgr, enum PlayerType type, const Ogre::Vector3& pos)
{
	graphicsEngine = mSceneMgr;
	visible = true;
	playShootAnimation = false;
	parentNode = graphicsEngine->getRootSceneNode()->createChildSceneNode(pos);
	playerNode = parentNode->createChildSceneNode();
	if (type == HUMAN) {
		playerEnt = mSceneMgr->createEntity("ninja.mesh");
		walkAnimation = playerEnt->getAnimationState("Walk");
		shootAnimation = playerEnt->getAnimationState("Attack3");
		playerNode->attachObject(playerEnt);
		Ogre::Real ratio = HEIGHT/195.649;
		playerNode->scale(ratio, ratio, ratio);
	} else {
		playerEnt = mSceneMgr->createEntity("robot.mesh");
		walkAnimation = playerEnt->getAnimationState("Walk");
		shootAnimation = playerEnt->getAnimationState("Shoot");
		playerNode->attachObject(playerEnt);
		playerNode->yaw(Ogre::Degree(90));
		Ogre::Real ratio = HEIGHT/101.673;
		playerNode->scale(ratio, ratio, ratio);
	}
	playerEnt->setCastShadows(true);

        walkAnimation->setLoop(true);
        walkAnimation->setEnabled(true);
        shootAnimation->setLoop(false);
        shootAnimation->setEnabled(true);
	// called after init
}

//-------------------------------------------------------------------------------------

ClientPlayer::~ClientPlayer(void) {
	playerNode->detachObject(playerEnt);
	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity(playerEnt);
	std::cout << "========= Debug: ClientPlayer Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ClientPlayer::toggleVisible(void) {
	if (visible) {
		playerNode->detachObject(playerEnt);
		visible = false;
	} else {
		playerNode->attachObject(playerEnt);
		visible = true;
	}
}

//-------------------------------------------------------------------------------------

void ClientPlayer::move(const Ogre::Vector3& pos, const Ogre::Quaternion& q, bool isMoving, bool isHitting) {
	setPosition(pos);
	setOrientation(q);
	if(isMoving) walkAnimation->addTime(1.0/60);

	if (playShootAnimation)	{
		double diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
		if (diff > shootAnimation->getLength()) {
			playShootAnimation =false;
			shootAnimation->setTimePosition(0);
		} else 
			shootAnimation->addTime(1.0/120);
	} else if (isHitting) {
		playShootAnimation = true;
		start = std::clock();
	}


	
}


