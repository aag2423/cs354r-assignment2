#include "Target.h"

Target::Target(Ogre::SceneManager* mSceneMgr, PhysicsEngine& physicsEngine, PlayGround* box, const Ogre::Vector3& pos, const Ogre::String& texture, Ogre::Real x, Ogre::Real y, Ogre::Real z) :
	parentNode(0),
	visible(true),
	graphicsEngine(mSceneMgr),
	obj(NULL),
	court(box),
	length(x),
	height(y),
	width(z),
	start(0),
	score(0),
	engine(&physicsEngine)
{
	obj = mSceneMgr->createEntity("cube.mesh");
	obj->setCastShadows(true);
	obj->setMaterialName(texture);
	parentNode = box->getNode()->createChildSceneNode(pos);
	parentNode->scale(length, height, width);
	parentNode->attachObject(obj);
	physicsObject.setToBox(
		btVector3(50*x, 50*y, 50*z),
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
	engine->removeObject(&physicsObject);
	parentNode->removeAndDestroyAllChildren();
	graphicsEngine->destroyEntity(obj);
	std::cout << "========= Debug: Target Deleted =========" << std::endl;
}

void Target::toggleVisible(void) {
	if (visible) {
		engine->removeObject(&physicsObject);
		parentNode->detachObject(obj);
		visible = false;
	} else {
		engine->addObject(&physicsObject);
		parentNode->attachObject(obj);
		visible = true;
	}
}
