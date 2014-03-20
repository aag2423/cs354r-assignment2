

#include "ClientCourt.h"
#include <iostream>

ClientCourt::ClientCourt(Ogre::SceneManager* mSceneMgr, PlayGroundType type) :
	courtType(type),
	graphicsEngine(mSceneMgr),
	parentNode(0),
	courtNum(0)
{
	parentNode = graphicsEngine->getRootSceneNode()->createChildSceneNode();
	setup();
}

//-------------------------------------------------------------------------------------

ClientCourt::~ClientCourt(void) {
	destroy();
	std::cout << "========= Debug: ClientCourt Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ClientCourt::toggleType(void) {
	destroy();
	courtType = (courtType == FULL_COURT) ? PRACTICE_COURT : FULL_COURT;
	setup();
	std::cout << "========= Debug: Play Ground Type Changed =========" << std::endl;
}

void ClientCourt::toggleCourt(const int court) {
	destroy();
	courtNum = court;
	setup();
	std::cout << "========= Debug: Play Ground Type Changed =========" << std::endl;
}
void ClientCourt::setup(void) {
	l = (courtType == FULL_COURT) ? FULL_COURT_LENGTH : PRACTICE_COURT_LENGTH;
	w = (courtType == FULL_COURT) ? FULL_COURT_WIDTH : PRACTICE_COURT_WIDTH;
	h = (courtType == FULL_COURT) ? FULL_COURT_HEIGHT : PRACTICE_COURT_HEIGHT;
	Ogre::SceneNode* wallNode = parentNode->createChildSceneNode("WallNode");
	if(courtNum == 0) {
		makePlane("wallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h, wallNode);
		makePlane("wallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h, wallNode);
		makePlane("floor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, (courtType == FULL_COURT) ? "Examples/TennisCourt" : "Examples/GrassFloor", h/2, w, l, wallNode);
		makePlane("ceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Flare", h/2, w, l, wallNode);
		makePlane("walll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h, wallNode);
		makePlane("wallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h, wallNode);
	}
	else if(courtNum == 1) {
		makePlane("wallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/Tron", l/2, w, h, wallNode);
		makePlane("wallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/Tron", l/2, w, h, wallNode);
		makePlane("floor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Grid", h/2, w, l, wallNode);
		makePlane("ceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Grid", h/2, w, l, wallNode);
		makePlane("walll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/Tron", w/2, l, h, wallNode);
		makePlane("wallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/Tron", w/2, l, h, wallNode);
	}
	else {
		makePlane("wallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", l/2, w, h, wallNode);
		makePlane("wallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", l/2, w, h, wallNode);
		makePlane("floor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/CloudySky", h/2, w, l, wallNode);
		makePlane("ceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/CloudySky", h/2, w, l, wallNode);
		makePlane("walll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", w/2, l, h, wallNode);
		makePlane("wallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", w/2, l, h, wallNode);
	}
	if (courtType == FULL_COURT) {
		Ogre::SceneNode* temp = parentNode->createChildSceneNode("NetNode", Ogre::Vector3(0,-h/2+20, 0));
		makePlane("net", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/TennisNet", 0, w, 40, temp);

	}
}

void ClientCourt::destroy(void) {

	parentNode->removeAndDestroyChild("WallNode");
	graphicsEngine->destroyEntity("wallf");
	graphicsEngine->destroyEntity("wallb");
	graphicsEngine->destroyEntity("walll");
	graphicsEngine->destroyEntity("wallr");
	graphicsEngine->destroyEntity("ceiling");
	graphicsEngine->destroyEntity("floor");
	if (courtType == FULL_COURT) {
		parentNode->removeAndDestroyChild("NetNode");
		graphicsEngine->destroyEntity("net");
	}
	Ogre::MeshManager::getSingleton().remove("wallf");
	Ogre::MeshManager::getSingleton().remove("wallb");
	Ogre::MeshManager::getSingleton().remove("walll");
	Ogre::MeshManager::getSingleton().remove("wallr");
	Ogre::MeshManager::getSingleton().remove("ceiling");
	Ogre::MeshManager::getSingleton().remove("floor");
}

//-------------------------------------------------------------------------------------

void ClientCourt::makePlane(std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int length, const int height, Ogre::SceneNode* parent) {
	Ogre::Real tile = 5;
	if(texture == "Examples/TennisCourt")
		tile = 1;
	Ogre::Plane plane(planeNormal, -distance);
	Ogre::MeshManager::getSingleton().createPlane(
		name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        	plane, length, height, 1, 1, true, 1, tile, tile, textureUp
	);
	Ogre::Entity* entSide = graphicsEngine->createEntity(name);
	if (parent == NULL)
		parentNode->attachObject(entSide);
	else
		parent->attachObject(entSide);
	entSide->setMaterialName(texture);
	entSide->setCastShadows(false);
}
