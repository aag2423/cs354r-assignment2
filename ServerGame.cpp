#include "ServerGame.h"
ServerGame::ServerGame(Ogre::SceneManager* mSceneMgr, GameMode mode, Gravity g, Ogre::Real ballRestitution) {
	graphicsEngine = mSceneMgr;
	physicsEngine.setGravity(0, -15, 0);
	sBall = NULL;
	sPlayer1 = NULL;
	sPlayer2 = NULL;
	gameState.paused = false;
	isFullGame = mode == FULL_GAME;
	sCourt = new ServerCourt(physicsEngine, isFullGame? FULL_COURT: PRACTICE_COURT);
	initData.gameMode = mode;
	restart();
	gameState.playerScore = 0;
	gameState.computerScore = 0;
	gameState.progress = ENDED;
	gameState.result = ONGOING;
}

//-------------------------------------------------------------------------------------

ServerGame::~ServerGame(void) {
	delete sCourt;
	delete sBall;
	delete sPlayer1;
	delete sPlayer2;
	std::cout << "========= Debug: GameServer Deleted =========" << std::endl;
}

//-------------------------------------------------------------------------------------


void ServerGame::restart(void) {
	delete sBall;
	delete sPlayer1;
	delete sPlayer2;

	Ogre::Vector3 pos;
	sCourt->getBallInitialPosition(pos);
	sBall = new ServerBall(physicsEngine, pos);


	sCourt->getPlayerInitialPosition(pos);
	sPlayer1 = new ServerPlayer(graphicsEngine, physicsEngine, HUMAN, pos, SIDE_NEAR);
	if (isFullGame) pos.z = -pos.z;
	else pos.x = -pos.x;
	sPlayer2 = new ServerPlayer(graphicsEngine, physicsEngine, AI, pos, SIDE_FAR, isFullGame);
	gameState.gameStarted = false;
	gameState.progress = NOT_STARTED;
	outputPositionState();
	outputPauseState();
}

void ServerGame::outputPositionState(void) {
	outputState.sceneState.playerNearPos = sPlayer1->getPosition();
	outputState.sceneState.playerFarPos = sPlayer2->getPosition();
	outputState.sceneState.playerNearOrient = sPlayer1->getOrientation();
	outputState.sceneState.playerFarOrient = sPlayer2->getOrientation();
	outputState.sceneState.ballPos = sBall->getPosition();
	outputState.sceneState.ballOrient = sBall->getOrientation();
}

void ServerGame::outputPauseState(void) {
	outputState.collisionEvent = NOTHING_HAPPENED;
	outputState.sceneState.playerNearHitting = false;
	outputState.sceneState.playerFarHitting = false;
	outputState.sceneState.playerNearMoving = false;
	outputState.sceneState.playerFarMoving = false;
}

void ServerGame::runAI(void) {
	Ogre::Vector3 ballPos = sBall->getPosition();
	Ogre::Vector3 aIPos = sPlayer2->getPosition();
	Ogre::Real x = ballPos.x - aIPos.x;
	sPlayer2->playerState.movingLeft = x > 0;
	sPlayer2->playerState.movingRight = x < 0;
	if (aIPos.z > ballPos.z)
		sPlayer2->playerState.movingBackward = true;
	else {
		sPlayer2->playerState.movingBackward = false;
		sPlayer2->playerState.movingForward = aIPos.z < -300;
	}
}


void ServerGame::runNextFrame(void) {
	if (gameState.progress == ENDED) {restart(); return;}
	if (gameState.paused || !gameState.gameStarted) {
		outputPositionState();
		outputPauseState();
		return;
	}

	Ogre::Vector3 range;
	sCourt->getHalfMoveRange(range);
	bool p1Moving = sPlayer1->move(range, isFullGame);
	bool p2Moving = sPlayer2->move(range, isFullGame);

	physicsEngine.stepSimulation(0.01);

	if (isFullGame && sPlayer2->playerType == AI) runAI();

	if(sBall->hitByPlayer(sPlayer1)) outputState.collisionEvent = HIT_PLAYER_SHOT;
	else if (sBall->hitByPlayer(sPlayer2)) outputState.collisionEvent = HIT_OPPONENT_SHOT;
	else outputState.collisionEvent = sBall->checkCollision(sCourt, sPlayer1, sPlayer2);
	if(isFullGame) checkScoring(outputState.collisionEvent);
	outputPositionState();
	outputState.sceneState.playerNearMoving = p1Moving;
	outputState.sceneState.playerFarMoving = p2Moving;
	outputState.sceneState.playerNearHitting =sPlayer1->playerState.hitting;
	outputState.sceneState.playerFarHitting =sPlayer2->playerState.hitting;
	outputState.gameState.playerScore = gameState.playerScore;
	outputState.gameState.opponentScore = gameState.computerScore;
	outputState.gameState.combo = gameState.comboBonus;
}

//-------------------------------------------------------------------------------------

void ServerGame::receive(InputState input) {
	if (input.playerState.serverPlayerSide == SIDE_NEAR)
		sPlayer1->playerState = input.playerState;
	else
		sPlayer2->playerState = input.playerState;
	if (input.startRound) { gameState.gameStarted = true;}
}

//-------------------------------------------------------------------------------------

OutputState ServerGame::send(bool isClientNearSide) {
	outputState.sceneState.isClientNearSide = isClientNearSide;
	return outputState;
}

//-------------------------------------------------------------------------------------

void ServerGame::checkScoring(BallCollisionEvent ballEvent) {
	Ogre::Real ballPosZ = sBall->getPosition().z;
	if (gameState.progress == NOT_STARTED) {
		switch(ballEvent){
			case HIT_PLAYER_SHOT:
				gameState.progress = HIT_BY_PLAYER; break;
			case HIT_OPPONENT_SHOT:
				gameState.progress = HIT_BY_OPPONENT; break;
			default: break;
		}
	} else if (gameState.progress == HIT_BY_PLAYER) {
		switch(ballEvent){
			case HIT_CEILING:
			case HIT_WALL: 
			case HIT_PLAYER:
				gameState.computerScore++; gameState.progress = ENDED; break;
			case HIT_FLOOR: 
				if (ballPosZ > 0){
					gameState.computerScore++; gameState.progress = ENDED;
				}else
					gameState.progress = HIT_BY_PLAYER_AND_FLOOR; 
				break;
			case HIT_OPPONENT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			case HIT_OPPONENT_SHOT:
				gameState.progress = HIT_BY_OPPONENT; break;
			default: break;
		}
	} else if (gameState.progress == HIT_BY_PLAYER_AND_FLOOR) {
		switch(ballEvent){
			case HIT_WALL:
			case HIT_FLOOR:
			case HIT_CEILING:
			case HIT_OPPONENT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			case HIT_PLAYER:
			case HIT_PLAYER_SHOT:
				gameState.computerScore++; gameState.progress = ENDED; break;
			case HIT_OPPONENT_SHOT:
				gameState.progress = HIT_BY_OPPONENT; break;
			default:break;
		}
	} else if (gameState.progress == HIT_BY_OPPONENT) {
		switch(ballEvent){
			case HIT_CEILING:
			case HIT_WALL: 
			case HIT_OPPONENT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			case HIT_FLOOR: 
				if (ballPosZ < 0){
					gameState.playerScore++; gameState.progress = ENDED;
				}else
					gameState.progress = HIT_BY_OPPONENT_AND_FLOOR; 
				break;
			case HIT_PLAYER:
				gameState.computerScore++; gameState.progress = ENDED; break;
			case HIT_PLAYER_SHOT:
				gameState.progress = HIT_BY_PLAYER; break;
			default: break;
		}
	} else if (gameState.progress == HIT_BY_OPPONENT_AND_FLOOR) {
		switch(ballEvent){
			case HIT_WALL:
			case HIT_FLOOR:
			case HIT_CEILING:
			case HIT_PLAYER:
				gameState.computerScore++; gameState.progress = ENDED; break;
			case HIT_OPPONENT:
			case HIT_OPPONENT_SHOT:
				gameState.playerScore++; gameState.progress = ENDED; break;
			case HIT_PLAYER_SHOT:
				gameState.progress = HIT_BY_PLAYER; break;
			default:break;
		}
	}

}
