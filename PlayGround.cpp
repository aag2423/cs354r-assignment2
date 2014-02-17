

#include "PlayGround.h"

PlayGround::PlayGround(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, Ogre::Real l, Ogre::Real w, Ogre::Real h) :
	parentNode(0)
{
	parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	makePlane(mSceneMgr, "wallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h);
	makePlane(mSceneMgr, "wallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h);
	makePlane(mSceneMgr, "floor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/GrassFloor", h/2, w, l);
	makePlane(mSceneMgr, "ceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_X, "Examples/Flare", h/2, w, l);
	makePlane(mSceneMgr, "walll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h);
	makePlane(mSceneMgr, "wallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h);


	bottomPlane.setToStaticPlane(btVector3(0,1,0), -h/2);
	topPlane.setToStaticPlane(btVector3(0,-1,0), -h/2);
	leftPlane.setToStaticPlane(btVector3(1,0,0), -w/2);
	rightPlane.setToStaticPlane(btVector3(-1,0,0), -w/2);
	frontPlane.setToStaticPlane(btVector3(0,0,-1), -l/2);
	backPlane.setToStaticPlane(btVector3(0,0,1), -l/2);
	//bottomPlane.setFriction(0.5);  
	//topPlane.setFriction(0.5);  
	bottomPlane.setRestitution(0.9); 
	topPlane.setRestitution(0.5); 
	leftPlane.setRestitution(0.8); 
	rightPlane.setRestitution(0.8); 
	frontPlane.setRestitution(0.8); 
	backPlane.setRestitution(0.8); 
	physicsEngine.addObject(&bottomPlane);
	physicsEngine.addObject(&topPlane);
	physicsEngine.addObject(&leftPlane);
	physicsEngine.addObject(&rightPlane);
	physicsEngine.addObject(&frontPlane);
	physicsEngine.addObject(&backPlane);
}

//-------------------------------------------------------------------------------------

PlayGround::~PlayGround(void) {
}
//-------------------------------------------------------------------------------------

void PlayGround::makePlane(Ogre::SceneManager* mSceneMgr, std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int length, const int height) {
	Ogre::Plane plane(planeNormal, -distance);
	Ogre::MeshManager::getSingleton().createPlane(
		name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, length, height, 1, 1, true, 1, 5, 5, textureUp
	);
	Ogre::Entity* entSide = mSceneMgr->createEntity(name);
	parentNode->attachObject(entSide);
	entSide->setMaterialName(texture);
	entSide->setCastShadows(false);
}
