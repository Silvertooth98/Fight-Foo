#include <iostream>
#include <time.h>
#include "EndState.h"
#include "Game.h"
#include "InputManager.h"
#include "MenuState.h"
#include "PlayState.h"
#include "Tools.h"

//------------------------------------------------------------------------------------------------------
//constructor that assigns all defaults
//------------------------------------------------------------------------------------------------------
PlayState::PlayState(GameState* state) : GameState(state)
{

	m_image = nullptr;
	
}
//------------------------------------------------------------------------------------------------------
//function that creates all game objects for the state
//------------------------------------------------------------------------------------------------------
bool PlayState::OnEnter()
{
	m_server.Init(1234);

	BinaryWriting();

	m_HealthHUD = new HealthHUD;
	m_roundHUD = new RoundHUD;
	m_serverMessagesHUD = new ServerMessages;

	m_roundHUD->resetRounds();

	m_endGAMEText = new GAMEtext;

	m_gameEnd = false;

	m_image = new Background("Assets\\Textures\\BG.png", "Assets\\Music\\Fight-Music.ogg");

	m_player = new Player();
	m_clientPlayer = new ConnectedPlayer();

	return true;
}
//------------------------------------------------------------------------------------------------------
//function that reads key presses, mouse clicks and updates all game objects in scene
//------------------------------------------------------------------------------------------------------
bool PlayState::Update()
{
	//play the background music associated with the image
	//when the state transitions to the next state stop it
	m_image->PlayMusic();

	//if a client isn't connected, make a listen
	//thread to listen for incoming clients
	if (!m_isClientConnected && !m_isListening)
	{
		if (!m_gameEnd)
		{
			m_serverMessagesHUD->SetHUDText("Waiting for Players");

			m_listenThread = std::thread(&PlayState::ListenForClient, this);
			m_listenThread.detach();
			m_isListening = true;
			std::cout << "Listening for clients" << std::endl;
		}
	}

	//if a client is connected, but the threads are yet to be
	//created, create the send and receive threads in order
	//to send messages to the client and receive messages from the client
	if (m_isClientConnected && !m_threadsCreated)
	{
		m_serverMessagesHUD->SetHUDText("Player Connected");

		m_sendThread = std::thread(&PlayState::SendMessage, this);
		m_recvThread = std::thread(&PlayState::ReceiveMessage, this);

		m_sendThread.detach();
		m_recvThread.detach();

		m_threadsCreated = true;
		std::cout << "Send and Receive Threads Created" << std::endl;
	}

	//the M key moves to the main menu
	if (keys[SDL_SCANCODE_M])
	{
		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		TheGame::Instance()->ChangeState(new MenuState(this));
	}

	//the Q key moves to the ending state
	if (keys[SDL_SCANCODE_Q])
	{
		if (m_isClientConnected == true)
		{
			m_server.SendMessage("Q");
			SDL_Delay(500);
			m_isClientConnected = false;
			m_server.Close();
			m_server.ShutDown();
		}

		m_gameEnd = true;

		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		TheGame::Instance()->ChangeState(new EndState(this));
	}

	//run the SetHealth function to setup the HUD
	SetHealth();

	if (/*m_isClientConnected == true &&*/ m_gameEnd == false)
	{
		//update the player (the server player)
		m_player->Update();

		//run the check health function for both the player and opponent
		CheckPlayerHealth();
		CheckOpponentHealth();

		//update the opponent (the client player)
		m_clientPlayer->Update();

		//run the player collision function
		PlayerCollision();

		//run the player input function
		PlayerInput();
	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that renders all game objects in scene
//------------------------------------------------------------------------------------------------------
bool PlayState::Draw()
{

	//render the background image
	m_image->Draw();

	//Add your code here...

	m_HealthHUD->Draw();
	m_roundHUD->Draw();
	m_serverMessagesHUD->Draw();

	if (m_showGAME == true)
	{
		m_endGAMEText->Draw();
	}

	m_player->Draw();
	m_clientPlayer->Draw();
	
	return true;
}
//------------------------------------------------------------------------------------------------------
//function that listens for any clients joining the game
//------------------------------------------------------------------------------------------------------
void PlayState::ListenForClient()
{
	m_server.Open();

	while (!m_server.Listen() && m_gameEnd == false)
	{
		SDL_Delay(100);
		std::cout << ".";
	}

	m_isListening = false;
	m_isClientConnected = true;
}
//------------------------------------------------------------------------------------------------------
//function that sends messages
//------------------------------------------------------------------------------------------------------
void PlayState::SendMessage()
{
	while (m_isClientConnected == true)
	{
		if (!m_messageOut.empty())
		{
			m_server.SendMessage(m_messageOut.front());
			m_messageOut.pop();
		}
	}
}
//------------------------------------------------------------------------------------------------------
//function that breaks up any messages that is received
//------------------------------------------------------------------------------------------------------
void PlayState::BreakUpMessage()
{
	std::string m_token;

	std::vector<std::string> subStrings;

	ParseString(m_messageReceived, subStrings, "_");

	if (!subStrings.empty())
	{
		m_token = subStrings[0];

		//Token for Position (P for Position)
		if (m_token == "P")
		{
			m_clientPlayer->SetIsIdle(true);
			m_clientPlayer->Normal();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_clientPlayer->SetPosition(xPos, yPos);
		}

		//Token for Jump (J for Jump)
		if (m_token == "J")
		{
			m_clientPlayer->Jump();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_clientPlayer->SetPosition(xPos, yPos);
		}

		//Token for Kick (K for Kick)
		if (m_token == "K")
		{
			m_clientPlayer->SetIsKicking(true);
			m_clientPlayer->Kick();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_clientPlayer->SetPosition(xPos, yPos);
		}

		//Token for Damage to the players' health (PD for Kick Damage)
		if (m_token == "KD")
		{
			m_clientPlayer->SetIsKicking(true);
			m_clientPlayer->Kick();
			m_player->DamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_serverMessagesHUD->SetHUDText("Opponent Kicked Player");
		}

		//Token for Punch (H for Hit - AKA Punch)
		if (m_token == "H")
		{
			m_clientPlayer->SetIsPunching(true);
			m_clientPlayer->Punch();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_clientPlayer->SetPosition(xPos, yPos);
		}

		//Token for Damage to the players' health (PD for Punch Damage)
		if (m_token == "PD")
		{
			m_clientPlayer->SetIsPunching(true);
			m_clientPlayer->Punch();
			m_player->DamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_serverMessagesHUD->SetHUDText("Opponent Punched Player");
		}

		//Token for the Opponents super being used (S for Super)
		if (m_token == "S")
		{
			m_clientPlayer->SetIsUsingSuper(true);
			m_clientPlayer->HadoukenStance();
			m_clientPlayer->HadoukenBeam();
			std::cout << "Client uses super" << std::endl;
		}

		//Token for Damage to the players' health (SD for Super Damage)
		if (m_token == "SD")
		{
			m_clientPlayer->SetIsUsingSuper(true);
			m_clientPlayer->HadoukenStance();
			m_clientPlayer->HadoukenBeam();
			m_player->SuperDamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_serverMessagesHUD->SetHUDText("Opponent Hit Their Super");
		}

		//Token for Opponents Health (HP for Health Points)
		if (m_token == "HP")
		{
			m_opponentHealth = (int)(atoi(subStrings[1].c_str()));
		}

		//Token for Idle (I for Idle)
		if (m_token == "I")
		{
			m_clientPlayer->SetIsIdle(true);
			m_clientPlayer->Normal();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_opponentHealth = (int)(atoi(subStrings[3].c_str()));
			m_clientPlayer->SetPosition(xPos, yPos);
		}

		//Token for Quit (Q for Quit)
		if (m_token == "Q")
		{
			m_isClientConnected = false;
			m_isListening = true;
			ListenForClient();
			m_clientPlayer->SetPosition(800, 400);
			m_serverMessagesHUD->SetHUDText("Opponent Disconnected");
		}
	}	
}
//------------------------------------------------------------------------------------------------------
//function that receives any messages
//------------------------------------------------------------------------------------------------------
void PlayState::ReceiveMessage()
{
	while (m_isClientConnected == true)
	{
		m_isClientConnected = m_server.ReceiveMessage(m_messageReceived);

		m_messageIn.push(m_messageReceived);

		if (!m_messageIn.empty())
		{
			m_messageIn.push(m_messageReceived);
			BreakUpMessage();
		}
	}
}
//------------------------------------------------------------------------------------------------------
//function that sends an initial value for the player's starting health
//the function also displays the player's and opponent's health on the HUD
//------------------------------------------------------------------------------------------------------
void PlayState::SetHealth()
{
	m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
	m_messageOut.push(m_messageSent);
	m_HealthHUD->SetPlayerHealthText("Player Health: " + std::to_string((int)m_player->GetHealth()));
	m_HealthHUD->SetOpponentHealthText("Opponent Health: " + std::to_string((int)m_opponentHealth));
}
//------------------------------------------------------------------------------------------------------
//function that checks the players health
//------------------------------------------------------------------------------------------------------
void PlayState::CheckPlayerHealth()
{
	if (m_player->GetHealth() == 0)
	{
		std::cout << "Round 1 Over, Player 2 wins" << std::endl;
		m_roundHUD->opponentWins();
		m_roundHUD->SetOpponentRoundText(std::to_string((int)m_roundHUD->GetOpponentWins()));
		m_clientPlayer->SetPosition(800, 400);
		m_player->SetPosition(10, 400);
		m_player->ResetHealth();
	}

	if (m_roundHUD->GetPlayerWins() == 2)
	{
		m_showGAME = true;
		std::cout << "Player 1 wins" << std::endl;
		m_roundHUD->resetRounds();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_roundHUD->SetOpponentRoundText(std::to_string((int)m_roundHUD->GetOpponentWins()));
		m_clientPlayer->SetPosition(800, 400);
		m_player->SetPosition(10, 400);
		m_player->ResetHealth();
	}
}
//------------------------------------------------------------------------------------------------------
//function that checks the opponents health
//------------------------------------------------------------------------------------------------------
void PlayState::CheckOpponentHealth()
{
	if (m_opponentHealth == 0)
	{
		std::cout << "Round 1 Over, Player 1 wins" << std::endl;
		m_roundHUD->playerWins();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_clientPlayer->SetPosition(800, 400);
		m_player->SetPosition(10, 400);
		m_player->ResetHealth();
	}
	
	if (m_roundHUD->GetOpponentWins() == 2)
	{
		m_showGAME = true;
		std::cout << "Player 2 wins" << std::endl;
		m_roundHUD->resetRounds();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_roundHUD->SetOpponentRoundText(std::to_string((int)m_roundHUD->GetOpponentWins()));
		m_clientPlayer->SetPosition(800, 400);
		m_player->SetPosition(10, 400);
		m_player->ResetHealth();
	}
}
//------------------------------------------------------------------------------------------------------
//function that checks if the player is colliding with the opponent
//------------------------------------------------------------------------------------------------------
void PlayState::PlayerCollision()
{
	//Run the check for collision function within the player's class
	//If it returns true, go into the if statement and say the player's are colliding
	if (m_player->CheckForCollision(m_clientPlayer))
	{
		std::cout << "PLAYER COLLISION" << std::endl;

		//Check if player is punching by running the getter in the player's class
		if (m_player->GetIsPunching() == true)
		{
			std::cout << "PUNCHING COLLISION!!!!" << std::endl;
			m_player->SetHitsLanded(m_player->GetHitsLanded() + 1);
			m_messageSent = "PD_";
			m_messageOut.push(m_messageSent);
			m_player->SetPosition(m_player->GetPosition().x - 30, m_player->GetPosition().y);
			m_clientPlayer->KnockBack();
		}

		//Check if player is kicking by running the getter in the player's class
		if (m_player->GetIsKicking() == true)
		{
			std::cout << "KICKING COLLISION!!!!" << std::endl;
			m_player->SetHitsLanded(m_player->GetHitsLanded() + 1);
			m_messageSent = "KD_";
			m_messageOut.push(m_messageSent);
			m_player->SetPosition(m_player->GetPosition().x - 30, m_player->GetPosition().y);
			m_clientPlayer->KnockBack();
		}
	}

	if (m_player->CheckForSuperCollision(m_clientPlayer))
	{
		std::cout << "SUPER COLLISION" << std::endl;

		//Check if the player is using their super by running the getter in the player's class
		if (m_player->GetIsUsingSuper() == true)
		{
			std::cout << "COLLISION WITH SUPER!!!" << std::endl;
			m_player->SetHitsLanded(0);
			m_messageSent = "SD_";
			m_messageOut.push(m_messageSent);
			m_player->SetPosition(m_player->GetPosition().x, m_player->GetPosition().y);
			m_clientPlayer->KnockBack();
		}
	}
}
//------------------------------------------------------------------------------------------------------
//function for the player input
//------------------------------------------------------------------------------------------------------
void PlayState::PlayerInput()
{
	//Check if the player presses right or left to move the player right or left
	//If they did, send a message to the client with the token "P" and then the
	//X-Pos and Y-Pos of the player
	if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_LEFT])
	{
		m_isPlayerMoving = true;
		m_messageSent = "P_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "MOVE" << std::endl;
	}
	//Check if the player presses space to make the player jump
	//If they did, send a message to the client with the token "J" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_SPACE] && m_player->GetIsFalling() == false)
	{
		m_messageSent = "J_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "JUMP" << std::endl;
	}
	//Check if the player presses Z to make the player kick
	//If they did, send a message to the client with the token "K" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_Z] && m_isPlayerMoving == false)
	{
		m_messageSent = "K_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "KICK" << std::endl;
	}
	//Check if the player presses X to make the player punch right
	//If they did, send a message to the client with the token "H" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_X] && m_isPlayerMoving == false)
	{
		m_messageSent = "H_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "PUNCH" << std::endl;
	}
	//Check if the player presses S to use the player SUPER
	//If they did, send a message to the client with the token "S" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_S] && m_isPlayerMoving == false && m_player->CanUseSuper() == true)
	{
		m_messageSent = "S_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "SUPER" << std::endl;
	}
	//Check if the player released any key
	//If they did, send a message to the client with the token "I" and then the
	//X-Pos and Y-Pos of the player, as well as the player's health
	else
	{
		m_isPlayerMoving = false;
		m_messageSent = "I_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY()) + "_" + std::to_string(m_player->GetHealth());
		m_messageOut.push(m_messageSent);
	}
}
//------------------------------------------------------------------------------------------------------
//function for writing binary into a text file
//------------------------------------------------------------------------------------------------------
void PlayState::BinaryWriting()
{
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "READING FROM THE BINARY FILE" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

	gPos = 0;
	pPos = 0;

	std::fstream binaryFile("Data/BinaryFile.txt", std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	if (!binaryFile)
	{
		//error
		std::cout << "File does not exist" << std::endl;
	}
	else
	{
		gPos = binaryFile.tellg();
		pPos = binaryFile.tellp();

		std::string welcomeMessage = "WELCOME TO FIGHT FUU!!";
		binaryFile.write(welcomeMessage.c_str(), welcomeMessage.size() + 1);

		int startHealth = 100;
		binaryFile.write((char*)&startHealth, sizeof(int));

		int startYPos = 800;
		binaryFile.write((char*)&startYPos, sizeof(int));

		int startXPos = 10;
		binaryFile.write((char*)&startXPos, sizeof(int));

		int healthNum;
		binaryFile.seekg(23);
		binaryFile.read((char*)&healthNum, sizeof(int));
		std::cout << "Player's Starting Health = " << healthNum << std::endl;

		int yPosValue;
		binaryFile.seekg(27);
		binaryFile.read((char*)&yPosValue, sizeof(int));
		std::cout << "Player's Starting Y Position = " << yPosValue << std::endl;

		int xPosValue;
		binaryFile.seekg(31);
		binaryFile.read((char*)&xPosValue, sizeof(int));
		std::cout << "Player's Starting X Position = " << xPosValue << std::endl;
	}
	binaryFile.close();
	std::cout << "---------------------------------------------------------------" << std::endl;
}
//------------------------------------------------------------------------------------------------------
//function that removes all game objects from memory
//------------------------------------------------------------------------------------------------------
void PlayState::OnExit()
{
	delete m_player;
	delete m_clientPlayer;
	delete m_endGAMEText;
	delete m_serverMessagesHUD;
	delete m_roundHUD;
	delete m_HealthHUD;
	delete m_image;
	m_server.ShutDown();
}