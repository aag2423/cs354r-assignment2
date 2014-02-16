
#include "Assignment2.h"
#include <OgreMath.h>
#include <OgreSphere.h>
#include <iostream>
//-------------------------------------------------------------------------------------
Assignment2::Assignment2(void) : 
	box(NULL),
	balls(),
	pauses(),
	mPaused(false),
	mouseClicked(false),
	bounces(0),
	collisions(0),
	mRotate(0.15),
	mMove(250),
	mDirection(Ogre::Vector3::ZERO)
{
}
//-------------------------------------------------------------------------------------
Assignment2::~Assignment2(void)
{
}
//-------------------------------------------------------------------------------------

void Assignment2::updatePanel(void) {
	scorePanel->setParamValue(MAX_NUM_BALLS, Ogre::StringConverter::toString(bounces));
	scorePanel->setParamValue(MAX_NUM_BALLS+1, Ogre::StringConverter::toString(collisions));
	scorePanel->setParamValue(MAX_NUM_BALLS+2, Ogre::StringConverter::toString(Ball::getSpeed()));
	for(int i = 0; i < MAX_NUM_BALLS; i++) {
		if(balls[i] == NULL)
			scorePanel->setParamValue(i, "Not set up");
		else if(pauses[i])
			scorePanel->setParamValue(i, "Paused");
		else
			scorePanel->setParamValue(i, "Moving");
	}

}
//-------------------------------------------------------------------------------------

void Assignment2::moveBall(const Ogre::FrameEvent& evt) {
	btTransform trans;
	Ogre::Vector3 dir = mCamNode->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	dir *= 10000;
	Ogre::Vector3 cameraPos = mCamNode->getPosition();
	Ogre::Vector3 ballPos = balls[0]->getNode()->getPosition();
	Ogre::Vector3 distance = Ogre::Vector3(cameraPos.x - ballPos.x, cameraPos.y - ballPos.y, cameraPos.z - ballPos.z);
	if(mouseClicked && (distance.x < 80.0 && distance.x > -80.0) && (distance.y < 80.0 && distance.y > -80.0) && (distance.z < 80.0 && distance.z > -80.0)) {
		ball.applyCentralForce(btVector3(dir.x, dir.y, dir.z));
	}
	physicsEngine.stepSimulation(evt.timeSinceLastFrame*4);
	
	ball.getWorldTransform(trans);
	btQuaternion orientation = trans.getRotation();
	balls[0]->getNode()->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
	balls[0]->getNode()->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));

	return;

	//dynamicsWorld->stepSimulation(1/60.f);
	dynamicsWorld->stepSimulation(evt.timeSinceLastFrame*5);
	fallRigidBody->getMotionState()->getWorldTransform(trans);
	
	return;
	for(int i = 0; i < MAX_NUM_BALLS; i++) {
		if(balls[i] == NULL)
			continue;
		for(int j = i+1; j < MAX_NUM_BALLS; j++) {
			if(balls[j] != NULL && balls[i]->collideWith(balls[j]))
				collisions++;
		}
		if(balls[i]->bounceWall()) bounces++;
		if(!pauses[i])
			balls[i]->move(evt);
	}
}

//-------------------------------------------------------------------------------------

void Assignment2::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MainCamNode", Ogre::Vector3(0,-100,200));
        mCamNode->attachObject(mCamera);
        //mCamNode->pitch(Ogre::Degree(360));
}
//-------------------------------------------------------------------------------------

void Assignment2::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	box = new PlayGround(mSceneMgr, BOX_SIDE_LENGTH, 0, 0, 0);
	balls[0] = new Ball(mSceneMgr, *box, 0, 0, 0);
/*
	balls[0] = new Ball(mSceneMgr, *box, 20, -20, 0);
	balls[1] = new Ball(mSceneMgr, *box, 20, 20, 0);
	balls[2] = new Ball(mSceneMgr, *box, -20, -20, 0);
	balls[3] = new Ball(mSceneMgr, *box, -20, 20, 0);
	balls[4] = new Ball(mSceneMgr, *box, 40, 0, 0);
	balls[5] = new Ball(mSceneMgr, *box, -40, 0, 0);
	balls[6] = new Ball(mSceneMgr, *box, 0, 40, 0);
	balls[7] = new Ball(mSceneMgr, *box, 0, -40, 0);
	balls[8] = new Ball(mSceneMgr, *box, 0, 0, 40);
	balls[9] = new Ball(mSceneMgr, *box, 0, 0, -40);
*/	
	Ball::setSpeed(1000);

	Ogre::Light* spotLight1 = mSceneMgr->createLight("spotLight1");
    	spotLight1->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight1->setDiffuseColour(Ogre::ColourValue::Blue);
	spotLight1->setSpecularColour(Ogre::ColourValue::Blue);
	spotLight1->setDirection(1, -1, -1);
	spotLight1->setPosition(Ogre::Vector3(-80, 80, -80));
	spotLight1->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(20));
	
	Ogre::Light* spotLight2 = mSceneMgr->createLight("spotLight2");
    	spotLight2->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight2->setDiffuseColour(Ogre::ColourValue::White);
	spotLight2->setSpecularColour(Ogre::ColourValue::White);
	spotLight2->setDirection(-1, -1, -1);
	spotLight2->setPosition(Ogre::Vector3(80, 80, -80));
	spotLight2->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(20));

	Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 80, -80));
	pointLight->setDiffuseColour(0.8, 0.8, 0.8);
	pointLight->setSpecularColour(0.8, 0.8, 0.8);





	// set up physical world
	ball.setToSphere(
		10, 
		2, 
		btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),
		btVector3(0,0,0) 
	);
	ball.setRestitution(0.99);
	ball.setLinearVelocity(btVector3(0,-98.1,0));
	ball.setFriction(0.5);  
	ball.setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));

	bottomPlane.setToStaticPlane(btVector3(0,1,0), -125);
	topPlane.setToStaticPlane(btVector3(0,-1,0), -125);
	leftPlane.setToStaticPlane(btVector3(1,0,0), -250);
	rightPlane.setToStaticPlane(btVector3(-1,0,0), -250);
	frontPlane.setToStaticPlane(btVector3(0,0,-1), -375);
	backPlane.setToStaticPlane(btVector3(0,0,1), -375);
	//bottomPlane.setFriction(0.5);  
	//topPlane.setFriction(0.5);  
	bottomPlane.setRestitution(0.8); 
	topPlane.setRestitution(0.8); 
	leftPlane.setRestitution(0.8); 
	rightPlane.setRestitution(0.8); 
	frontPlane.setRestitution(0.8); 
	backPlane.setRestitution(0.8); 

	physicsEngine.addObject(&ball);
	physicsEngine.addObject(&bottomPlane);
	physicsEngine.addObject(&topPlane);
	physicsEngine.addObject(&leftPlane);
	physicsEngine.addObject(&rightPlane);
	physicsEngine.addObject(&frontPlane);
	physicsEngine.addObject(&backPlane);
	physicsEngine.setGravity(0, -98.1, 0);


/*
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-10,0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-BOX_SIDE_LENGTH/2,0)));
	btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setRestitution(0.9);
	dynamicsWorld->addRigidBody(groundRigidBody);


	groundShape = new btStaticPlaneShape(btVector3(0,-1,0),0);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,BOX_SIDE_LENGTH/2,0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI2(0,groundMotionState,groundShape,btVector3(0,0,0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI2);
	groundRigidBody->setRestitution(0.9);
	groundRigidBody->setFriction(0.5);
	dynamicsWorld->addRigidBody(groundRigidBody);








	btCollisionShape* fallShape = new btSphereShape(10);
	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.2f, 0.6f, 0.1f, 1.0f).normalized(),btVector3(0,0,0)));
	btScalar mass = 1;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
	fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setLinearVelocity(btVector3(0,-10,0));
	fallRigidBody->setRestitution(0.99); 
	fallRigidBody->setFriction(0.5);  
	fallRigidBody->setAngularVelocity(btVector3(0.2f, 0.5f, 0.2f));
        dynamicsWorld->addRigidBody(fallRigidBody);

*/
	
}
//-------------------------------------------------------------------------------------
void Assignment2::createFrameListener(void) {
	BaseApplication::createFrameListener();    
	Ogre::StringVector items;
     	items.push_back("Ball 1");
     	items.push_back("Ball 2");
     	items.push_back("Ball 3");
     	items.push_back("Ball 4");
     	items.push_back("Ball 5");
     	items.push_back("Ball 6");
     	items.push_back("Ball 7");
     	items.push_back("Ball 8");
     	items.push_back("Ball 9");
     	items.push_back("Ball 10");
     	items.push_back("Bounces");	
     	items.push_back("Collisions");
     	items.push_back("Ball Speed");
	scorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "ScorePanel", 150, items);
	updatePanel();
}
//-------------------------------------------------------------------------------------
bool Assignment2::frameStarted(const Ogre::FrameEvent& evt)
{
        return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;
	if (mShutDown) return false;
	mKeyboard->capture();
	mMouse->capture();
	mTrayMgr->frameRenderingQueued(evt);
	mCamNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	moveBall(evt);
	updatePanel();
        return true;
}
//-------------------------------------------------------------------------------------
// OIS::KeyListener
bool Assignment2::keyPressed( const OIS::KeyEvent& evt ){
	switch (evt.key) {
		case OIS::KC_ESCAPE: 
			mShutDown = true;
			break;
		case OIS::KC_SPACE: 
			mPaused = !mPaused;
			for(int i = 0; i < MAX_NUM_BALLS; i++) pauses[i] = mPaused;
			break;
		case OIS::KC_A:
    			mDirection.x = -mMove;
    			break;
		case OIS::KC_D:
    			mDirection.x = mMove;
    			break;
		case OIS::KC_Q:
    			mDirection.y = mMove;
    			break;
		case OIS::KC_E:
    			mDirection.y = -mMove;
    			break;
		case OIS::KC_W:
    			mDirection.z = -mMove;
    			break;
		case OIS::KC_S:
    			mDirection.z = mMove;
    			break;
		case OIS::KC_1:
    			pauses[0] = !pauses[0];
    			break;
		case OIS::KC_2:
    			pauses[1] = !pauses[1];
    			break;
		case OIS::KC_3:
    			pauses[2] = !pauses[2];
    			break;
		case OIS::KC_4:
    			pauses[3] = !pauses[3];
    			break;
		case OIS::KC_5:
    			pauses[4] = !pauses[4];
    			break;
		case OIS::KC_6:
    			pauses[5] = !pauses[5];
    			break;
		case OIS::KC_7:
    			pauses[6] = !pauses[6];
    			break;
		case OIS::KC_8:
    			pauses[7] = !pauses[7];
    			break;
		case OIS::KC_9:
    			pauses[8] = !pauses[8];
    			break;
		case OIS::KC_0:
    			pauses[9] = !pauses[9];
    			break;
		case OIS::KC_ADD:
		case OIS::KC_EQUALS:
			Ball::setSpeed(Ball::getSpeed() + 10);
    			break;
		case OIS::KC_MINUS:
		case OIS::KC_SUBTRACT:
			Ball::setSpeed(Ball::getSpeed() - 10);
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::keyReleased( const OIS::KeyEvent& evt ){
	switch (evt.key) {
		case OIS::KC_A:
		case OIS::KC_D:
    			mDirection.x = 0;
    			break;
		case OIS::KC_Q:
		case OIS::KC_E:
    			mDirection.y = 0;
    			break;
		case OIS::KC_W:
		case OIS::KC_S:
    			mDirection.z = 0;
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------

// OIS::MouseListener
bool Assignment2::mouseMoved( const OIS::MouseEvent& evt ){
	mCamNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
    	mCamNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	switch (id)
	{
	case OIS::MB_Left:
		mouseClicked = true;
		break;
	default:
	    break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	switch (id)
	{
	case OIS::MB_Left:
		mouseClicked = false;
		break;
	default:
	    break;
	}
	return true;
}
//-------------------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Assignment2 app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
