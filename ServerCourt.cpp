

#include "ServerCourt.h"
#include <iostream>

ServerCourt::ServerCourt(PhysicsEngine& engine, PlayGroundType type) :
	courtType(type),
	physicsEngine(engine)
{
	setup();
}

//-------------------------------------------------------------------------------------

ServerCourt::~ServerCourt(void) {
	destroy();
	std::cout << "========= Debug: Play Ground Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ServerCourt::toggleType(void) {
	destroy();
	courtType = (courtType == FULL_COURT) ? PRACTICE_COURT : FULL_COURT;
	setup();
	std::cout << "========= Debug: Play Ground Type Changed =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ServerCourt::setup(void) {
	l = (courtType == FULL_COURT) ? FULL_COURT_LENGTH : PRACTICE_COURT_LENGTH;
	w = (courtType == FULL_COURT) ? FULL_COURT_WIDTH : PRACTICE_COURT_WIDTH;
	h = (courtType == FULL_COURT) ? FULL_COURT_HEIGHT : PRACTICE_COURT_HEIGHT;
	if (courtType == FULL_COURT) {
		net.setToBox(
			btVector3(w/2, 20, 2), 
			0, 
			btQuaternion(0,0,0,1), 
			btVector3(0, -h/2+20, 0));
		physicsEngine.addObject(&net);
		net.setRestitution(0.001); 

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
	physicsEngine.addObject(&bottomPlane);
	physicsEngine.addObject(&topPlane);
	physicsEngine.addObject(&leftPlane);
	physicsEngine.addObject(&rightPlane);
	physicsEngine.addObject(&frontPlane);
	physicsEngine.addObject(&backPlane);
}

//-------------------------------------------------------------------------------------

void ServerCourt::destroy(void) {
	physicsEngine.removeObject(&bottomPlane);
	physicsEngine.removeObject(&topPlane);
	physicsEngine.removeObject(&leftPlane);
	physicsEngine.removeObject(&rightPlane);
	physicsEngine.removeObject(&frontPlane);
	physicsEngine.removeObject(&backPlane);
	if (courtType == FULL_COURT) {
		physicsEngine.removeObject(&net);
	}
}
