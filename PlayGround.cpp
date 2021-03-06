

#include "PlayGround.h"
#include <iostream>

PlayGround::PlayGround(Ogre::SceneManager* mSceneMgr, PhysicsEngine& engine, PlayGroundType type) :
	courtType(type),
	graphicsEngine(mSceneMgr),
	physicsEngine(&engine),
	parentNode(0),
	courtNum(0)
{
	parentNode = graphicsEngine->getRootSceneNode()->createChildSceneNode();
	setup();
}

//-------------------------------------------------------------------------------------

PlayGround::~PlayGround(void) {
	destroy();
	std::cout << "========= Debug: Play Ground Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void PlayGround::toggleType(void) {
	destroy();
	courtType = (courtType == FULL_COURT) ? PRACTICE_COURT : FULL_COURT;
	setup();
	std::cout << "========= Debug: Play Ground Type Changed =========" << std::endl;
}

void PlayGround::toggleCourt(const int court) {
	destroy();
	courtNum = court;
	setup();
	std::cout << "========= Debug: Play Ground Type Changed =========" << std::endl;
}
void PlayGround::setup(void) {
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
		Ogre::SceneNode* netF = parentNode->createChildSceneNode("NetNode1", Ogre::Vector3(0,-h/2+20, 0));
		Ogre::SceneNode* netB = parentNode->createChildSceneNode("NetNode2", Ogre::Vector3(0,-h/2+20, 0));
		makePlane("net1",-Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/TennisNet", 0, w, 40, netF);
		makePlane("net2",-Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, "Examples/TennisNet", 0, w, 40, netB);
		netFront.setToBox(
			btVector3(w/2, 20, 2), 
			0, 
			btQuaternion(0,0,0,1), 
			btVector3(0, -h/2+20, 0));
		physicsEngine->addObject(&netFront);
		netBack.setToBox(
			btVector3(w/2, 20, 2), 
			0, 
			btQuaternion(0,0,0,1), 
			btVector3(0, -h/2+20, 0));
		physicsEngine->addObject(&netBack);

	}
	bottomPlane.setToStaticPlane(btVector3(0,1,0), -h/2);
	topPlane.setToStaticPlane(btVector3(0,-1,0), -h/2);
	leftPlane.setToStaticPlane(btVector3(1,0,0), -w/2);
	rightPlane.setToStaticPlane(btVector3(-1,0,0), -w/2);
	frontPlane.setToStaticPlane(btVector3(0,0,-1), -l/2);
	backPlane.setToStaticPlane(btVector3(0,0,1), -l/2);
	//bottomPlane.setFriction(0.5);  
	//topPlane.setFriction(0.5);  
	leftPlane.setFriction(0.9);  
	rightPlane.setFriction(0.9);  
	bottomPlane.setRestitution(0.8); 
	topPlane.setRestitution(0.5); 
	leftPlane.setRestitution(0.8); 
	rightPlane.setRestitution(0.8); 
	frontPlane.setRestitution(0.1); 
	backPlane.setRestitution(0.8); 
	netFront.setRestitution(0.02); 
	netBack.setRestitution(0.02);
	physicsEngine->addObject(&bottomPlane);
	physicsEngine->addObject(&topPlane);
	physicsEngine->addObject(&leftPlane);
	physicsEngine->addObject(&rightPlane);
	physicsEngine->addObject(&frontPlane);
	physicsEngine->addObject(&backPlane);
}

void PlayGround::destroy(void) {
	physicsEngine->removeObject(&bottomPlane);
	physicsEngine->removeObject(&topPlane);
	physicsEngine->removeObject(&leftPlane);
	physicsEngine->removeObject(&rightPlane);
	physicsEngine->removeObject(&frontPlane);
	physicsEngine->removeObject(&backPlane);

	parentNode->removeAndDestroyChild("WallNode");
	graphicsEngine->destroyEntity("wallf");
	graphicsEngine->destroyEntity("wallb");
	graphicsEngine->destroyEntity("walll");
	graphicsEngine->destroyEntity("wallr");
	graphicsEngine->destroyEntity("ceiling");
	graphicsEngine->destroyEntity("floor");
	if (courtType == FULL_COURT) {
		parentNode->removeAndDestroyChild("NetNode1");
		graphicsEngine->destroyEntity("net1");
		physicsEngine->removeObject(&netFront);
		parentNode->removeAndDestroyChild("NetNode2");
		graphicsEngine->destroyEntity("net2");
		physicsEngine->removeObject(&netBack);
	}
	Ogre::MeshManager::getSingleton().remove("wallf");
	Ogre::MeshManager::getSingleton().remove("wallb");
	Ogre::MeshManager::getSingleton().remove("walll");
	Ogre::MeshManager::getSingleton().remove("wallr");
	Ogre::MeshManager::getSingleton().remove("ceiling");
	Ogre::MeshManager::getSingleton().remove("floor");
}

//-------------------------------------------------------------------------------------

void PlayGround::makePlane(std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int length, const int height, Ogre::SceneNode* parent) {
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
