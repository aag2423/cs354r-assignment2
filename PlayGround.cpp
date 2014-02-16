

#include "PlayGround.h"

PlayGround::PlayGround(Ogre::SceneManager* mSceneMgr, Ogre::Real sideLength, Ogre::Real x, Ogre::Real y, Ogre::Real z) :
	parentNode(0),
	size(sideLength/2)
{
	parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x, y, z));
	makePlane(mSceneMgr, "wallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 375, 500, 250);
	makePlane(mSceneMgr, "wallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 375, 500, 250);
	makePlane(mSceneMgr, "floor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/GrassFloor", 125, 500, 750);
	makePlane(mSceneMgr, "ceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_X, "Examples/Flare", 125, 750, 500);
	makePlane(mSceneMgr, "walll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 250, 750, 250);
	makePlane(mSceneMgr, "wallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 250, 750, 250);
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
