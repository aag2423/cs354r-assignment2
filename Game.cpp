#include "Game.h"

#include <OgreFrameListener.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <iostream>

Game::Game(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNode) {

	gameState.camMode = ABOVE_CAM;
	gameState.paused = false;
	gameState.gameStarted = false;

	physicsEngine.setGravity(0, EARTH_G, 0);

	playerState.step = NORMAL_STEP;
	playerState.strength = NORMAL_HIT;
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
	playerState.hitting = false;
	playerState.shotDirection = 0;

	graphicsEngine = mSceneMgr;
	cameraNode = camNode;

	court = new PlayGround(mSceneMgr, physicsEngine, 750, 500, 250);
	ball = new Ball(mSceneMgr, physicsEngine, court, Ogre::Vector3(0, -80, 180));
	player = new Player(mSceneMgr, physicsEngine, court, Ogre::Vector3(0, -125, 200));
	toggleCamera();
}

//-------------------------------------------------------------------------------------

Game::~Game(void) {
	delete ball;
	delete court;
	delete player;
	std::cout << "========= Debug: Game Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void Game::reset(void) {
	physicsEngine.setGravity(0, EARTH_G, 0);

	playerState.step = NORMAL_STEP;
	playerState.strength = NORMAL_HIT;
	playerState.movingLeft = false;
	playerState.movingRight = false;
	playerState.movingForward = false;
	playerState.movingBackward = false;
	playerState.hitting = false;
	playerState.shotDirection = 0;

	ball->setPositionAndPause(Ogre::Vector3(0, -80, 180)); //this should set position both in physics engine and Ogre scene
	player->setPosition(Ogre::Vector3(0, -125, 200)); // same as above

	gameState.gameStarted = false;
}

//-------------------------------------------------------------------------------------

void Game::serveBall(void) {
	if(!gameState.gameStarted)
		ball->resume();
	gameState.gameStarted = true;
}

//-------------------------------------------------------------------------------------

void Game::runNextFrame(const Ogre::FrameEvent& evt) {
	if(gameState.paused) return;
	movePlayer(evt);
	if(playerState.hitting) ball->hitBy(player, playerState.shotDirection);
	physicsEngine.stepSimulation(evt.timeSinceLastFrame*10);
	ball->updateGraphicsScene();
}

//-------------------------------------------------------------------------------------

void Game::toggleCamera(void) {
	Ogre::Node* oldCamParent = cameraNode->getParent();
	oldCamParent->removeChild(cameraNode);
	Ogre::SceneNode* newCamParent;
	if(gameState.camMode == ABOVE_CAM) {
		gameState.camMode = FIRST_PERSON_CAM;
		newCamParent = player->getNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 180, 0);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-15));
	} else {
		gameState.camMode = ABOVE_CAM;
		newCamParent = graphicsEngine->getRootSceneNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 125, 500);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-20));
	}
}

//-------------------------------------------------------------------------------------

void Game::movePlayer(const Ogre::FrameEvent& evt) {
	if (!gameState.gameStarted) return;
	Ogre::Vector3 direction(0, 0, 0);
	if (playerState.movingLeft)
		direction.x -= playerState.step;
	if (playerState.movingRight)
		direction.x += playerState.step;
	if (playerState.movingForward)
		direction.z -= playerState.step;
	if (playerState.movingBackward)
		direction.z += playerState.step;
	
	// check if player runs off court, move according to direction
	player->move(direction*evt.timeSinceLastFrame);
}

//-------------------------------------------------------------------------------------

void Game::handleKeyboardEvent(enum KeyboardEvent evt) {
	switch (evt) {
	case GO_LEFT:
		playerState.movingLeft = true; break;
	case GO_RIGHT:
		playerState.movingRight = true; break;
	case GO_FORWARD:
		playerState.movingForward = true; break;
	case GO_BACKWARD:
		playerState.movingBackward = true; break;
	case STOP_LEFT:
		playerState.movingLeft = false; break;
	case STOP_RIGHT:
		playerState.movingRight = false; break;
	case STOP_FORWARD:
		playerState.movingForward = false; break;
	case STOP_BACKWARD:
		playerState.movingBackward = false; break;
	case RUN:
	        playerState.step = RUN_STEP; break;
	case STOP_RUN:
	        playerState.step = NORMAL_STEP; break;
	case USE_WEAK_HIT:
        	playerState.strength = WEAK_HIT; break;
	case USE_NORMAL_HIT:
	        playerState.strength = NORMAL_HIT; break;
	case USE_STRONG_HIT:
	        playerState.strength = STRONG_HIT; break;
	case TOGGLE_CAMERA:
		toggleCamera(); 
		break;
	case RESTART:
		reset(); break;
	case PAUSE:
		gameState.paused = !gameState.paused; break;
	default: break;
	}
}

void Game::handleMouseMove(Ogre::Real dx, Ogre::Real dy) {
	if (playerState.hitting) {
		if (dx > 0)
			playerState.shotDirection.x = BACKHAND;
		else if (dx < 0)
                        playerState.shotDirection.x = FOREHAND;
		if (dy > 0) 
			playerState.shotDirection.y = VOLLEY;
		else if (dy < 0) 
			playerState.shotDirection.y = SMASH;
	}
	if (gameState.camMode == FIRST_PERSON_CAM){
		player->getNode()->yaw(Ogre::Degree(-0.15*dx), Ogre::Node::TS_LOCAL);
		cameraNode->pitch(Ogre::Degree(-0.15*dy), Ogre::Node::TS_LOCAL);
	} 
}

void Game::handleMouseClick(enum MouseEvent evt) {
	switch (evt) {
	case HIT_START:
		playerState.hitting = true;
		break;
	case HIT_STOP:
		playerState.hitting = false;
		playerState.shotDirection = 0; 
		break;
	default: break;
	}
}

