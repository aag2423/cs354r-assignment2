/*
-----------------------------------------------------------------------------
Filename:    CubeBox.cpp
-----------------------------------------------------------------------------
Jiawei Guo, jg44347
-----------------------------------------------------------------------------
*/

#include "CubeBox.h"

CubeBox::CubeBox(Ogre::SceneManager* mSceneMgr, Ogre::Real sideLength, Ogre::Real x, Ogre::Real y, Ogre::Real z) :
	parentNode(0),
	halfSideLength(sideLength/2)
{
	parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x, y, z));
	makePlane(mSceneMgr, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN");
	makePlane(mSceneMgr, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN");
	makePlane(mSceneMgr, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Rockwall");
	makePlane(mSceneMgr, Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/MRAMOR6X6");
	makePlane(mSceneMgr, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN");
	makePlane(mSceneMgr, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN");
}

//-------------------------------------------------------------------------------------

CubeBox::~CubeBox(void) {
}
//-------------------------------------------------------------------------------------

void CubeBox::makePlane(Ogre::SceneManager* mSceneMgr, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture) {
	Ogre::Plane plane(planeNormal, -halfSideLength);
	Ogre::String meshName = Ogre::StringConverter::toString(Ogre::Math::RangeRandom(0, 200.0f));
	Ogre::MeshManager::getSingleton().createPlane(
		meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, 2*halfSideLength, 2*halfSideLength, 1, 1, true, 1, 5, 5, textureUp
	);
	Ogre::Entity* entSide = mSceneMgr->createEntity(meshName);
	parentNode->attachObject(entSide);
	entSide->setMaterialName(texture);
	entSide->setCastShadows(false);
}
