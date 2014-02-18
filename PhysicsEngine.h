#ifndef __PhysicsEngine_h_
#define __PhysicsEngine_h_

#include "PhysicsObject.h"
#include <btBulletDynamicsCommon.h>

class PhysicsEngine
{
protected:
	btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfig;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
public:
	PhysicsEngine(void);
	~PhysicsEngine(void);
	void addObject(PhysicsObject* obj);
	void removeObject(PhysicsObject* obj);
	void setGravity(btScalar x, btScalar y, btScalar z);
	void stepSimulation(btScalar step, int maxSubSteps = 1, btScalar fixStep = 1.0f/60.0f);
	btDiscreteDynamicsWorld* getPhysicsWorld(void) { return dynamicsWorld; }

};
#endif // #ifndef __PhysicsEngine_h_
