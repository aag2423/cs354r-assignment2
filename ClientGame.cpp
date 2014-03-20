#include "ClientGame.h"
ClientGame::ClientGame(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* camNode, InitializationData& iniData, OutputState initServerState) {
	graphicsEngine = mSceneMgr;
	isServerFullGame = iniData.gameMode == FULL_GAME;
	cCourt = new ClientCourt(graphicsEngine, (isServerFullGame) ? FULL_COURT : PRACTICE_COURT);

	localPlayerState.serverPlayerSide = iniData.isClientNearSide ? SIDE_NEAR : SIDE_FAR;
	localPlayerState.strength = STRONG_HIT;
	localPlayerState.step = NORMAL_STEP;
	localPlayerState.shotDirection = 0;
	localPlayerState.yaw = 0;
	localPlayerState.movingLeft = false;
	localPlayerState.movingRight = false;
	localPlayerState.movingForward = false;
	localPlayerState.movingBackward = false;
	localPlayerState.hitting = false;

	serverState = initServerState;


	isNearSide = iniData.isClientNearSide;
	cPlayer1 = new ClientPlayer(graphicsEngine, HUMAN);
	cPlayer2 = new ClientPlayer(graphicsEngine, AI);
	cBall = new ClientBall(graphicsEngine);

	std::cout << "========= Debug: ClientasdsadsadadGame Deleted =========" << std::endl;

	cameraNode = camNode;
	camMode = ABOVE_CAM;
	toggleCamera();

	std::cout << "========= Debug: ClientasdsadsadadGame Deleted =========" << std::endl;
	//soundPlayer = new Sound();
	soundHandler->start_ambient();
}

//-------------------------------------------------------------------------------------

ClientGame::~ClientGame(void) {
	Ogre::Node* oldCamParent = cameraNode->getParent();
	oldCamParent->removeChild(cameraNode);
	graphicsEngine->getRootSceneNode()->addChild(cameraNode);
	delete cCourt;
	delete cBall;
	delete cPlayer1;
	delete cPlayer2;
	//delete soundPlayer;
	std::cout << "========= Debug: ClientGame Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------

void ClientGame::toggleCamera(void) {
	Ogre::Node* oldCamParent = cameraNode->getParent();
	oldCamParent->removeChild(cameraNode);
	Ogre::SceneNode* newCamParent;
	if(camMode == ABOVE_CAM) {
		camMode = THIRD_PERSON_CAM;
		newCamParent = cPlayer1->getSceneNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 80, 50);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-10));
	} else if (camMode == THIRD_PERSON_CAM){
		camMode = FIRST_PERSON_CAM;
		newCamParent = cPlayer1->getSceneNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 60, 0);
		cameraNode->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-15));
	} else {
		camMode = ABOVE_CAM;
		newCamParent = graphicsEngine->getRootSceneNode();
		newCamParent->addChild(cameraNode);
		cameraNode->setPosition(0, 50, 700);
		cameraNode->resetOrientation();
		cPlayer1->getSceneNode()->resetOrientation();
        	cameraNode->pitch(Ogre::Degree(-10));
	}
}

//-------------------------------------------------------------------------------------

InputState ClientGame::send(void) {
	InputState result;
	result.startRound = localPlayerState.hitting;
	result.playerState = localPlayerState;
	localPlayerState.yaw = 0;
	localPlayerState.shotDirection = 0;
	localPlayerState.hitting = false;
	return result;
}


void ClientGame::runNextFrame(void) {
	switch(serverState.collisionEvent) {
		case HIT_PLAYER_SHOT:
		case HIT_OPPONENT_SHOT:
			soundHandler->play_sound(ball_hit); break;
		case HIT_PLAYER:
		case HIT_OPPONENT: 
			soundHandler->play_sound(grunt); break;
		case HIT_CEILING:
		case HIT_FLOOR:
		case HIT_WALL:
			soundHandler->play_sound(ball_hit_floor); break;
		default: break;
	}


	OutputSceneState* newScene = &(serverState.sceneState);
	cBall->setPosition(newScene->ballPos);
	cBall->setOrientation(newScene->ballOrient);
	if (isNearSide) {
		cPlayer1->move(
			newScene->playerNearPos, 
			newScene->playerNearOrient, 
			newScene->playerNearMoving,
			newScene->playerNearHitting
		);
		cPlayer2->move(
			newScene->playerFarPos, 
			newScene->playerFarOrient, 
			newScene->playerFarMoving,
			newScene->playerFarHitting
		);
	} else {
		cPlayer1->move(
			newScene->playerFarPos, 
			newScene->playerFarOrient, 
			newScene->playerFarMoving,
			newScene->playerFarHitting
		);
		cPlayer2->move(
			newScene->playerNearPos, 
			newScene->playerNearOrient, 
			newScene->playerNearMoving,
			newScene->playerNearHitting
		);
	}

	Ogre::Vector3 dir = cameraNode->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	localPlayerState.shotDirection.x = dir.x * 100;
	localPlayerState.shotDirection.y = (dir.y * 100) + 20;
}

//-------------------------------------------------------------------------------------

void ClientGame::receive(OutputState newState) {
	serverState = newState;
	
}

//-------------------------------------------------------------------------------------

void ClientGame::handleKeyboardEvent(enum KeyboardEvent evt) {
	switch (evt) {
	case GO_LEFT:
		localPlayerState.movingLeft = true; break;
	case GO_RIGHT:
		localPlayerState.movingRight = true; break;
	case GO_FORWARD:
		localPlayerState.movingForward = true; break;
	case GO_BACKWARD:
		localPlayerState.movingBackward = true; break;
	case STOP_LEFT:
		localPlayerState.movingLeft = false; break;
	case STOP_RIGHT:
		localPlayerState.movingRight = false; break;
	case STOP_FORWARD:
		localPlayerState.movingForward = false; break;
	case STOP_BACKWARD:
		localPlayerState.movingBackward = false; break;
	case RUN:
	        localPlayerState.step = RUN_STEP; break;
	case STOP_RUN:
	        localPlayerState.step = NORMAL_STEP; break;
	case USE_WEAK_HIT:
        	localPlayerState.strength = WEAK_HIT; break;
	case USE_NORMAL_HIT:
        	localPlayerState.strength = NORMAL_HIT; break;
	case USE_STRONG_HIT:
        	localPlayerState.strength = STRONG_HIT; break;
	case TOGGLE_CAMERA:
		toggleCamera();
	default: break;
	}
}

void ClientGame::handleMouseMove(Ogre::Real dx, Ogre::Real dy) {
	localPlayerState.yaw = -0.15*dx;
	if (camMode == FIRST_PERSON_CAM)
		cameraNode->pitch(Ogre::Degree(-0.15*dy), Ogre::Node::TS_LOCAL);
	//localPlayerState.shotDirection.x = dx*2;
	//dy = -dy*8;
	//localPlayerState.shotDirection.y = dy > 35 ? 35: dy;
}

void ClientGame::handleMouseClick(enum MouseEvent evt) {
//	if (gameState.paused || gameState.result != ONGOING) return;
	switch (evt) {
	case HIT_START:
		localPlayerState.hitting = true;
//		gameState.gameStarted = true;
		break;
	case HIT_STOP:
		localPlayerState.hitting = false;
		break;
	default: break;
	}
}
