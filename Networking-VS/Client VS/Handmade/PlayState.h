/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This game state controls the main state of the game. It consists of a background object which
  will render the main bacdrop image and play the correct background music. Other game objects 
  can be added to the vector as needed. 

- The Update() function will update all the game objects in the vector, as long as they are 
  active. The Draw() routine draws all the game objects in the vector, as long as they are active
  and visible. 

*/

#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "Background.h"
#include "GameState.h"
#include "Player.h"
#include "ServerPlayer.h"
#include "HUD.h"
#include "HealthBars.h"
#include "RoundHUD.h"
#include "ClientMessages.h"
#include "TCPClient.h"
#include "Tools.h"
#include "GAMEText.h"
#include <vector>
#include <thread>
#include <queue>
#include <fstream>

class PlayState : public GameState
{

public:

	PlayState(GameState* state);
	virtual ~PlayState() {}

public:

	virtual bool OnEnter();
	virtual bool Update();
	virtual bool Draw();
	virtual void OnExit();

private:

	void WaitingForServer();
	void SendMessage();
	void BreakUpMessage();
	void ReceiveMessage();

	void SetHealth();
	void CheckPlayerHealth();
	void CheckOpponentHealth();
	void PlayerCollision();
	void PlayerInput();
	void BinaryWriting();

	Background* m_image;
	HUD* m_HUD;
	HealthBars* m_healthBar;
	RoundHUD* m_roundHUD;
	ClientMessages* m_clientMessagesHUD;

	GAMEText* m_endGAMEText;

	Player* m_player;
	ServerPlayer* m_serverPlayer;

	std::vector<GameObject*> m_objects;

	bool m_isListening;
	bool m_threadsCreated;
	bool m_isClientConnected;
	bool m_isPlayerMoving;
	bool m_gameEnd;
	bool m_showGAME;

	std::thread m_sendThread;
	std::thread m_recvThread;

	std::queue<std::string> m_messageIn;
	std::queue<std::string> m_messageOut;

	std::string m_messageSent;
	std::string m_messageReceived;

	TCPClient m_client;

	//read keyboard state
	const Uint8* keys = TheInput::Instance()->GetKeyStates();

	int m_opponentHealth = 100;

	int gPos; //get pointer
	int pPos; //put pointer

};
#endif