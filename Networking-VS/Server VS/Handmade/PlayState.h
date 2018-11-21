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
#include "ConnectedPlayer.h"
#include "HUD.h"
#include "HealthHUD.h"
#include "RoundHUD.h"
#include "ServerMessages.h"
#include "TCPServer.h"
#include "GAMEtext.h"
#include <vector>
#include <thread>
#include <queue>
#include <deque>
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

	void ListenForClient();
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
	HealthHUD* m_HealthHUD;
	RoundHUD* m_roundHUD;
	ServerMessages* m_serverMessagesHUD;

	GAMEtext* m_endGAMEText;

	Player* m_player;
	ConnectedPlayer* m_clientPlayer;

	bool m_isListening;
	bool m_threadsCreated;
	bool m_isClientConnected;
	bool m_isPlayerMoving;
	bool m_gameEnd;
	bool m_showGAME;

	std::thread m_sendThread;
	std::thread m_recvThread;
	std::thread m_listenThread;

	std::string m_messageSent;
	std::string m_messageReceived;

	std::queue<std::string> m_messageIn;
	std::queue<std::string> m_messageOut;

	TCPServer m_server;

	//read keyboard state
	const Uint8* keys = TheInput::Instance()->GetKeyStates();
	
	int m_opponentHealth = 100;

	int gPos; //get pointer
	int pPos; //put pointer

};
#endif