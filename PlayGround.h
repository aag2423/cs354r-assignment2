
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
	BACK_PLANE
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
	void makePlane(Ogre::SceneManager* mSceneMgr, std::string name, const Ogre::Vector3& planeNormal, const Ogre::Vector3& textureUp, const Ogre::String& texture, const int distance, const int lenght, const int height);
protected:
	Ogre::SceneNode* parentNode;
	Ogre::Real halfLength;
	Ogre::Real halfWidth;
	Ogre::Real halfHeight;
	PhysicsObject bottomPlane;
	PhysicsObject topPlane;
	PhysicsObject leftPlane;
	PhysicsObject rightPlane;
	PhysicsObject frontPlane;
	PhysicsObject backPlane;
public:
	bool bRoll;
	PlayGround(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, Ogre::Real l, Ogre::Real w, Ogre::Real h);
	~PlayGround(void);
	Ogre::SceneNode* getNode(void) { return parentNode; }
	void getHalfDimension(Ogre::Vector3& result) { 
		result.y = halfWidth;
		result.z = halfHeight;
		result.x = halfLength;
	}
	void getHalfMovaRange(Ogre::Vector3& result) {
		result.x = halfWidth - 10;
		result.y = halfHeight - 20; 
		result.z = halfLength - 10; 
	}
	PhysicsObject& getPhysicsObject(PlaneNumber i) { 
		switch(i) {
		case BOTTOM_PLANE: return bottomPlane;
		case TOP_PLANE: return topPlane;
		case LEFT_PLANE: return leftPlane;
		case RIGHT_PLANE: return rightPlane;
		case FRONT_PLANE: return frontPlane;
		default: return backPlane;
		}
	}
};
#endif // #ifndef __PlayGround_h_
