#include "Target.h"

Target::Target(const Ogre::String& name, Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos, const Ogre::String& texture, Ogre::Real x, Ogre::Real y, Ogre::Real z) :
	parentNode(0),
	obj(NULL),
	court(box),
	length(x),
	height(y),
	width(z),
	start(0),
	score(0)
{
	obj = mSceneMgr->createEntity(name, "cube.mesh");
	obj->setCastShadows(true);
	obj->setMaterialName(texture);
	parentNode = box->getNode()->createChildSceneNode(pos);
	parentNode->scale(length, height, width);
	parentNode->attachObject(obj);
	physicsObject.setToBox(
		btVector3(40, 40, 20),
		100,
		btQuaternion(0, 0, 0, 1),
		btVector3(pos.x, pos.y, pos.z)
	);
	physicsObject.toggleRigidBodyAndKinematic();
	physicsObject.setRestitution(0.8);
	physicsObject.setFriction(1);
	physicsEngine.addObject(&physicsObject);
}

Target::~Target(void) {
	std::cout << "========= Debug: Target Deleted =========" << std::endl;
}
