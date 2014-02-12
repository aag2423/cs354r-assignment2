#ifndef __PhysicsObject_h_
#define __PhysicsObject_h_

#include <btBulletDynamicsCommon.h>

/*
Usage:
PhysicsObject po = new PhysicsObject();
po.setToXX(...) XX could be Sphere/Box/Plane
PhysicsEngine pe = new PhysicsEngine();
physicsEngine.addObject(po);
physicsEngine.removeObject(po);
delete po;

Before changing the object from one form to another (e.g., Sphere to Plane) or deleting it, be sure to delete the rigidbody from the physics engine if it's associated with one.
example:
*/
class PhysicsObject
{
private:
	void clearData(void);
protected:
	btCollisionShape* collisionShape;
	btRigidBody* rigidBody;
public:
	// Initializations
	PhysicsObject(void);
	virtual ~PhysicsObject(void);

	void setToBox(const btVector3 &boxHalfExtents, btScalar mass, const btQuaternion &orientation, const btVector3 &pos);
	void setToStaticPlane(const btVector3 &normal, btScalar distAlongNormal);
	void setToSphere(btScalar radius, btScalar mass, const btQuaternion &orientation, const btVector3 &pos);

	// Utility functions
	btRigidBody* getRigidBody(void) { return rigidBody; }
	void getWorldTransform(btTransform& trans);
	void setWorldTransform(const btTransform& newTrans);

	void applyCentralForce(const btVector3& force);
	void setAngularVelocity(const btVector3& v);
	void setFriction(btScalar factor);
	void setRestitution(btScalar factor);
	void setLinearVelocity(const btVector3& v);
	
};
#endif // #ifndef __PhysicsObject_h_
