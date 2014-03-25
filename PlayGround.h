
#ifndef __PlayGround_h_
#define __PlayGround_h_

#include <Ogre.h>
#include "PhysicsEngine.h"

enum PlaneNumber {
	BOTTOM_PLANE,
	TOP_PLANE,
	LEFT_PLANE,
	RIGHT_PLANE,
	FRONT_PLANE,
	BACK_PLANE,
	NET
};
enum PlayGroundType {
	FULL_COURT, PRACTICE_COURT
};
enum PlayGroundSize {
	FULL_COURT_LENGTH = 1600,
	FULL_COURT_WIDTH = 700,
	FULL_COURT_HEIGHT = 250,
	PRACTICE_COURT_LENGTH = 750,
	PRACTICE_COURT_WIDTH = 500,
	PRACTICE_COURT_HEIGHT = 250,
};

struct MyPlaneContactResultCallback : public btCollisionWorld::ContactResultCallback {
    MyPlaneContactResultCallback(void) : hit(false){}

    virtual btScalar addSingleResult(btManifoldPoint& cp,
	const btCollisionObject* colObj0, int partId0, int index0,
	const btCollisionObject* colObj1, int partId1, int index)
    {
	hit = true;
    }
	bool hit;
};

class PlayGround {
private:
	void makePlane(std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int lenght, const int height, Ogre::SceneNode* parent=NULL);
	void setup();
	void destroy(void);
protected:
	bool forServer;
	PlayGroundType courtType;
	Ogre::SceneManager* graphicsEngine;
	Ogre::SceneNode* parentNode;
	PhysicsEngine* physicsEngine;
	Ogre::Real l;
	Ogre::Real w;
	Ogre::Real h;
	PhysicsObject net;
	PhysicsObject bottomPlane;
	PhysicsObject topPlane;
	PhysicsObject leftPlane;
	PhysicsObject rightPlane;
	PhysicsObject frontPlane;
	PhysicsObject backPlane;
	int courtNum;
public:
	PlayGround(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGroundType type);
	~PlayGround(void);
	void toggleType(void);
	void toggleCourt(const int court);

	Ogre::SceneNode* getNode(void) { return parentNode; }
	void getHalfDimension(Ogre::Vector3& result) { 
		result.x = w/2;
		result.y = h/2;
		result.z = l/2;
	}
	void getHalfMovaRange(Ogre::Vector3& result) {
		result.x = w/2 - 10;
		result.y = h - 70; 
		result.z = l/2 - 10; 
	}
	void getBallInitialPosition(Ogre::Vector3& result) {
		result.x = 5;
		result.y = -(h - 70); 
		result.z = l/2 - 310; 
	}
	PhysicsObject& getPhysicsObject(PlaneNumber i) { 
		switch(i) {
		case BOTTOM_PLANE: return bottomPlane;
		case TOP_PLANE: return topPlane;
		case LEFT_PLANE: return leftPlane;
		case RIGHT_PLANE: return rightPlane;
		case FRONT_PLANE: return frontPlane;
		case BACK_PLANE: return backPlane;
		default: return net;
		}
	}
};
#endif // #ifndef __PlayGround_h_
