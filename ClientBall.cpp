#include "ClientBall.h"
#include <iostream>

ClientBall::ClientBall(Ogre::SceneManager* mSceneMgr, const Ogre::Vector3& pos) :  
	graphicsEngine(mSceneMgr)
{
	entBall = graphicsEngine->createEntity(Ogre::SceneManager::PT_SPHERE);
	entBall->setCastShadows(true);
	entBall->setMaterialName("Examples/TennisBall");

	parentNode = graphicsEngine->getRootSceneNode()->createChildSceneNode(pos);
	Ogre::SceneNode* tempNode = parentNode->createChildSceneNode();
	tempNode->attachObject(entBall);
	Ogre::Real ratio = BALL_RADIUS/100;
	tempNode->scale(ratio, ratio, ratio);
}

//-------------------------------------------------------------------------------------

ClientBall::~ClientBall(void) {
	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity("entBall");
	std::cout << "========= Debug: ClientBall Deleted =========" << std::endl;
}

