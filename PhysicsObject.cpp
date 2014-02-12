#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(void) : 
	collisionShape(NULL),
	rigidBody(NULL)
{
}

PhysicsObject::~PhysicsObject(void)
{
	clearData();
}


void PhysicsObject::clearData(void) {
	if(rigidBody)
		delete rigidBody->getMotionState();
	delete rigidBody;
	delete collisionShape;
}

void PhysicsObject::setToBox(const btVector3 &boxHalfExtents, btScalar mass, const btQuaternion &orientation, const btVector3 &pos) {
	clearData();

	collisionShape = new btBoxShape(boxHalfExtents);
        btVector3 inertia(0,0,0);
        collisionShape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* motionState = 
		new btDefaultMotionState(btTransform(orientation,pos));

        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,collisionShape,inertia);
        rigidBody = new btRigidBody(rigidBodyCI);
}


void PhysicsObject::setToStaticPlane(const btVector3 &normal, btScalar distAlongNormal) {
	clearData();
	collisionShape = new btStaticPlaneShape(normal, distAlongNormal);
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1)));
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0,motionState,collisionShape,btVector3(0,0,0));
        rigidBody = new btRigidBody(rigidBodyCI);
}


void PhysicsObject::setToSphere(btScalar radius, btScalar mass, const btQuaternion &orientation, const btVector3 &pos) {
	clearData();
	
	collisionShape = new btSphereShape(radius);
        btVector3 inertia(0,0,0);
        collisionShape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* motionState = 
		new btDefaultMotionState(btTransform(orientation,pos));

        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,motionState,collisionShape,inertia);
        rigidBody = new btRigidBody(rigidBodyCI);
}

void PhysicsObject::getWorldTransform(btTransform& trans) {
	rigidBody->getMotionState()->getWorldTransform(trans);
}
void PhysicsObject::setWorldTransform(const btTransform& newTrans) {
	rigidBody->getMotionState()->setWorldTransform(newTrans);
}

void PhysicsObject::applyCentralForce(const btVector3& force) {
	rigidBody->applyCentralForce(force);
}

void PhysicsObject::setRestitution(btScalar factor) {
	rigidBody->setRestitution(factor);
}

void PhysicsObject::setFriction(btScalar factor) {
	rigidBody->setFriction(factor);
}

void PhysicsObject::setLinearVelocity(const btVector3& v) {
	rigidBody->setLinearVelocity(v);
}

void PhysicsObject::setAngularVelocity(const btVector3& v) {
	rigidBody->setAngularVelocity(v);
}



