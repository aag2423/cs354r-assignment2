
#include "Assignment2.h"
#include <OgreMath.h>
#include <OgreSphere.h>
#include <iostream>
//-------------------------------------------------------------------------------------
Assignment2::Assignment2(void) : 
	game(NULL),
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
	delete game;
	std::cout << "========= Debug: Assignment2 Deleted =========" << std::endl;
}
//-------------------------------------------------------------------------------------

void Assignment2::updatePanel(void) {
	scorePanel->setParamValue(MAX_NUM_BALLS, Ogre::StringConverter::toString(bounces));
	scorePanel->setParamValue(MAX_NUM_BALLS+1, Ogre::StringConverter::toString(collisions));
	/*
	scorePanel->setParamValue(MAX_NUM_BALLS+2, Ogre::StringConverter::toString(Ball::getSpeed()));
	for(int i = 0; i < MAX_NUM_BALLS; i++) {
		if(balls[i] == NULL)
			scorePanel->setParamValue(i, "Not set up");
		else if(pauses[i])
			scorePanel->setParamValue(i, "Paused");
		else
			scorePanel->setParamValue(i, "Moving");
	}
	*/
}

//-------------------------------------------------------------------------------------

void Assignment2::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("MainCam");
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MainCamNode", Ogre::Vector3(0,100,300));
        mCamNode->attachObject(mCamera);
        //mCamNode->pitch(Ogre::Degree(360));
}
//-------------------------------------------------------------------------------------

void Assignment2::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	game = new Game(mSceneMgr, mCamNode);

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
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (mWindow->isClosed()) return false;
	if (mShutDown) return false;
	mKeyboard->capture();
	mMouse->capture();
	mTrayMgr->frameRenderingQueued(evt);
	mCamNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	game->runNextFrame(evt);
	updatePanel();
        return true;
}
//-------------------------------------------------------------------------------------
// OIS::KeyListener
bool Assignment2::keyPressed( const OIS::KeyEvent& evt ){
	switch (evt.key) {
		case OIS::KC_ESCAPE: 
			mShutDown = true;
			game->handleKeyboardEvent(PAUSE);
			break;
		case OIS::KC_SPACE: 
			mPaused = !mPaused;
			for(int i = 0; i < MAX_NUM_BALLS; i++) pauses[i] = mPaused;
			break;
		case OIS::KC_R:
			game->handleKeyboardEvent(RESTART);
    			break;
		case OIS::KC_J:
    			mDirection.x = -mMove;
    			break;
		case OIS::KC_L:
    			mDirection.x = mMove;
    			break;
		case OIS::KC_U:
    			mDirection.y = mMove;
    			break;
		case OIS::KC_O:
    			mDirection.y = -mMove;
    			break;
		case OIS::KC_I:
    			mDirection.z = -mMove;
    			break;
		case OIS::KC_K:
    			mDirection.z = mMove;
    			break;
		case OIS::KC_W:
			game->handleKeyboardEvent(GO_FORWARD);
    			break;
		case OIS::KC_S:
			game->handleKeyboardEvent(GO_BACKWARD);
    			break;
		case OIS::KC_A:
			game->handleKeyboardEvent(GO_LEFT);
    			break;
		case OIS::KC_D:
			game->handleKeyboardEvent(GO_RIGHT);
    			break;
		case OIS::KC_LCONTROL:
			game->handleKeyboardEvent(RUN);
    			break;
		case OIS::KC_C:
			game->handleKeyboardEvent(TOGGLE_CAMERA);
    			break;
		case OIS::KC_1:
			game->handleKeyboardEvent(USE_WEAK_HIT);
    			break;
		case OIS::KC_2:
			game->handleKeyboardEvent(USE_NORMAL_HIT);
    			break;
		case OIS::KC_3:
			game->handleKeyboardEvent(USE_STRONG_HIT);
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::keyReleased( const OIS::KeyEvent& evt ){
	switch (evt.key) {
		case OIS::KC_LCONTROL:
			game->handleKeyboardEvent(STOP_RUN);
    			break;
		case OIS::KC_J:
		case OIS::KC_L:
    			mDirection.x = 0;
    			break;
		case OIS::KC_U:
		case OIS::KC_O:
    			mDirection.y = 0;
    			break;
		case OIS::KC_I:
		case OIS::KC_K:
    			mDirection.z = 0;
    			break;
		case OIS::KC_W:
			game->handleKeyboardEvent(STOP_FORWARD);
    			break;
		case OIS::KC_S:
			game->handleKeyboardEvent(STOP_BACKWARD);
    			break;
		case OIS::KC_A:
			game->handleKeyboardEvent(STOP_LEFT);
    			break;
		case OIS::KC_D:
			game->handleKeyboardEvent(STOP_RIGHT);
    			break;
		default:
			break;
	}
	return true;
}
//-------------------------------------------------------------------------------------

// OIS::MouseListener
bool Assignment2::mouseMoved( const OIS::MouseEvent& evt ){
	game->handleMouseMove(evt.state.X.rel, evt.state.Y.rel);
//	mCamNode->yaw(Ogre::Degree(-mRotate * evt.state.X.rel), Ogre::Node::TS_WORLD);
//    	mCamNode->pitch(Ogre::Degree(-mRotate * evt.state.Y.rel), Ogre::Node::TS_LOCAL);
	return true;
}
//-------------------------------------------------------------------------------------
bool Assignment2::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id ){
	switch (id)
	{
	case OIS::MB_Left:
		game->serveBall();
		game->handleMouseClick(HIT_START);
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
		game->handleMouseClick(HIT_STOP);
		break;
	case OIS::MB_Right:
		//ball->getPhysicsObject().toggleRigidBodyAndKinematic(2);
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
