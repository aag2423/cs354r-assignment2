/*
-----------------------------------------------------------------------------
Filename:    CubeBox.cpp
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#include "PlayGround.h"

PlayGround::PlayGround(Ogre::SceneManager* mSceneMgr, Ogre::Real sideLength, Ogre::Real x, Ogre::Real y, Ogre::Real z) :
	parentNode(0),
	size(sideLength/2)
{
	parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x, y, z));
	makePlane(mSceneMgr, "wall1", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 3000, 3000, 3000);
	makePlane(mSceneMgr, "wall2", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 3000, 3000, 3000);
	makePlane(mSceneMgr, "ceiling", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Rockwall", 1500, 6000, 3000);
	makePlane(mSceneMgr, "floor", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/MRAMOR6X6", 1500, 6000, 3000);
	makePlane(mSceneMgr, "wall3", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 1500, 3000, 3000);
	makePlane(mSceneMgr, "wall4", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", 1500, 3000, 3000);
}

//-------------------------------------------------------------------------------------

PlayGround::~PlayGround(void) {
}
//-------------------------------------------------------------------------------------

void PlayGround::makePlane(Ogre::SceneManager* mSceneMgr, String name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int length, const int height) {
	Ogre::Plane plane(planeNormal, -distance);
	Ogre::MeshManager::getSingleton().createPlane(
		name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, length, height, 1, 1, true, 1, 5, 5, textureUp
	);
	Ogre::Entity* entSide = mSceneMgr->createEntity(meshName);
	parentNode->attachObject(entSide);
	entSide->setMaterialName(texture);
	entSide->setCastShadows(false);
}
