

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
	Ogre::SceneNode* wallNode = parentNode->createChildSceneNode();
	if(courtNum == 0) {
		makePlane("cwallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h, wallNode);
		makePlane("cwallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", l/2, w, h, wallNode);
		makePlane("cfloor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, (courtType == FULL_COURT) ? "Examples/TennisCourt" : "Examples/GrassFloor", h/2, w, l, wallNode);
		makePlane("cceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Flare", h/2, w, l, wallNode);
		makePlane("cwalll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h, wallNode);
		makePlane("cwallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/KAMEN", w/2, l, h, wallNode);
	}
	else if(courtNum == 1) {
		makePlane("cwallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/Tron", l/2, w, h, wallNode);
		makePlane("cwallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/Tron", l/2, w, h, wallNode);
		makePlane("cfloor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Grid", h/2, w, l, wallNode);
		makePlane("cceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/Grid", h/2, w, l, wallNode);
		makePlane("cwalll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/Tron", w/2, l, h, wallNode);
		makePlane("cwallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/Tron", w/2, l, h, wallNode);
	}
	else {
		makePlane("cwallf", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", l/2, w, h, wallNode);
		makePlane("cwallb", Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", l/2, w, h, wallNode);
		makePlane("cfloor", Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/CloudySky", h/2, w, l, wallNode);
		makePlane("cceiling", Ogre::Vector3::NEGATIVE_UNIT_Y, Ogre::Vector3::UNIT_Z, "Examples/CloudySky", h/2, w, l, wallNode);
		makePlane("cwalll", Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", w/2, l, h, wallNode);
		makePlane("cwallr", Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Y, "Examples/CloudySky", w/2, l, h, wallNode);
	}
	if (courtType == FULL_COURT) {
		Ogre::SceneNode* net = parentNode->createChildSceneNode(Ogre::Vector3(0,-h/2+20, 0));
		makePlane("cnet1", Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/TennisNet", 0, w, 40, net);
		makePlane("cnet2", -Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/TennisNet", 0, w, 40, net);
	}
}

void ClientCourt::destroy(void) {

	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity("cwallf");
	graphicsEngine->destroyEntity("cwallb");
	graphicsEngine->destroyEntity("cwalll");
	graphicsEngine->destroyEntity("cwallr");
	graphicsEngine->destroyEntity("cceiling");
	graphicsEngine->destroyEntity("cfloor");
	if (courtType == FULL_COURT) {
		graphicsEngine->destroyEntity("cnet1");
		graphicsEngine->destroyEntity("cnet2");
	}
	Ogre::MeshManager::getSingleton().remove("cwallf");
	Ogre::MeshManager::getSingleton().remove("cwallb");
	Ogre::MeshManager::getSingleton().remove("cwalll");
	Ogre::MeshManager::getSingleton().remove("cwallr");
	Ogre::MeshManager::getSingleton().remove("cceiling");
	Ogre::MeshManager::getSingleton().remove("cfloor");
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
