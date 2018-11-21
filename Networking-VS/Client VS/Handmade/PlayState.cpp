#include <iostream>
#include <time.h>
#include "EndState.h"
#include "Game.h"
#include "InputManager.h"
#include "MenuState.h"
#include "PlayState.h"
#include <thread>

std::string m_serverIP;

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
	std::cout << "Enter the server IP: ";
	std::cin >> m_serverIP;

	m_client.Init(1234, m_serverIP.c_str());

	if (!m_client.Open())
	{
		WaitingForServer();
	}

	BinaryWriting();

	m_healthBar = new HealthBars;
	m_roundHUD = new RoundHUD;
	m_clientMessagesHUD = new ClientMessages;

	m_endGAMEText = new GAMEText;

	m_gameEnd = false;

	m_image = new Background("Assets\\Textures\\BG.png", "Assets\\Music\\Fight-Music.ogg");

	m_player = new Player();
	m_serverPlayer = new ServerPlayer();

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

	//if the client is connected to the server, but the threads are yet to be
	//created, create the send and receive threads in order
	//to send messages to the server and receive messages from the server
	if (m_isClientConnected && !m_threadsCreated)
	{
		m_clientMessagesHUD->SetHUDText("Connected to Server");

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
		m_messageSent = "Q_TRUE";
		m_messageOut.push(m_messageSent);

		m_gameEnd = true;
		m_client.Close();

		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		TheGame::Instance()->ChangeState(new EndState(this));
	}

	//run the SetHealth function to setup the HUD
	SetHealth();

	if (m_isClientConnected == true && m_gameEnd == false)
	{
		//update the player (the client player)
		m_player->Update();

		//run the check health function for both the player and opponent
		CheckPlayerHealth();
		CheckOpponentHealth();

		//update the opponent (the server player)
		m_serverPlayer->Update();

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

	m_healthBar->Draw();
	m_roundHUD->Draw();
	m_clientMessagesHUD->Draw();

	if (m_showGAME == true)
	{
		m_endGAMEText->Draw();
	}

	m_player->Draw();
	m_serverPlayer->Draw();
	
	return true;
}
//------------------------------------------------------------------------------------------------------
//function that waits for a server to connect to
//------------------------------------------------------------------------------------------------------
void PlayState::WaitingForServer()
{
	while (!m_client.Open() && m_gameEnd == false)
	{
		SDL_Delay(100);
		std::cout << ".";
	}

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
			m_client.SendMessage(m_messageOut.front());
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
			m_serverPlayer->SetIsIdle(true);
			m_serverPlayer->Normal();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_serverPlayer->SetPosition(xPos, yPos);
		}

		//Token for Jump (J for Jump)
		if (m_token == "J")
		{
			m_serverPlayer->Jump();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_serverPlayer->SetPosition(xPos, yPos);
		}

		//Token for Kick (K for Kick)
		if (m_token == "K")
		{
			m_serverPlayer->SetIsKicking(true);
			m_serverPlayer->Kick();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_serverPlayer->SetPosition(xPos, yPos);
		}

		//Token for Damage to the players' health (KD for Kick Damage)
		if (m_token == "KD")
		{
			m_serverPlayer->SetIsKicking(true);
			m_serverPlayer->Kick();
			m_player->DamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_clientMessagesHUD->SetHUDText("Opponent Kicked Player");
		}

		//Token for Punch (H for Hit - AKA Punch)
		if (m_token == "H")
		{
			m_serverPlayer->SetIsPunching(true);
			m_serverPlayer->Punch();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_serverPlayer->SetPosition(xPos, yPos);
		}

		//Token for Damage to the players' health (PD for Punch Damage)
		if (m_token == "PD")
		{
			m_serverPlayer->SetIsPunching(true);
			m_serverPlayer->Punch();
			m_player->DamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_clientMessagesHUD->SetHUDText("Opponent Punched Player");
		}

		//Token for the Opponents super being used (S for Super)
		if (m_token == "S")
		{
			m_serverPlayer->SetIsUsingSuper(true);
			m_serverPlayer->HadoukenStance();
			m_serverPlayer->HadoukenBeam();
			std::cout << "Client uses super" << std::endl;
		}

		//Token for Damage to the players' health (SD for Super Damage)
		if (m_token == "SD")
		{
			m_serverPlayer->SetIsUsingSuper(true);
			m_serverPlayer->HadoukenStance();
			m_serverPlayer->HadoukenBeam();
			m_player->SuperDamageHealth();
			m_player->KnockBack();
			std::cout << "Health: " << m_player->GetHealth();
			m_messageSent = "HP_" + std::to_string((int)m_player->GetHealth());
			m_messageOut.push(m_messageSent);
			m_clientMessagesHUD->SetHUDText("Opponent Hit Their Super");
		}

		//Token for Opponents Health (HP for Health Points)
		if (m_token == "HP")
		{
			m_opponentHealth = (int)(atoi(subStrings[1].c_str()));
		}

		//Token for Idle (I for Idle)
		if (m_token == "I")
		{
			m_serverPlayer->SetIsIdle(true);
			m_serverPlayer->Normal();
			float xPos = (float)(atof(subStrings[1].c_str()));
			float yPos = (float)(atof(subStrings[2].c_str()));
			m_opponentHealth = (int)(atoi(subStrings[3].c_str()));
			m_serverPlayer->SetPosition(xPos, yPos);
		}

		//Token for Quit (Q for Quit)
		if (m_token == "Q")
		{
			m_isClientConnected = false;

			m_gameEnd = true;

			m_client.Close();

			m_image->StopMusic();
			m_isActive = m_isAlive = false;
			TheGame::Instance()->ChangeState(new EndState(this));
		}
	}
}
//------------------------------------------------------------------------------------------------------
//function that receives any messages
//------------------------------------------------------------------------------------------------------
void PlayState::ReceiveMessage()
{
	while (m_isClientConnected)
	{
		m_isClientConnected = m_client.ReceiveMessage(m_messageReceived);

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
	m_healthBar->SetPlayerHealthText("Player Health: " + std::to_string((int)m_player->GetHealth()));
	m_healthBar->SetOpponentHealthText("Opponent Health: " + std::to_string((int)m_opponentHealth));
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
		m_serverPlayer->SetPosition(10, 400);
		m_player->SetPosition(800, 400);
		m_player->ResetHealth();
	}

	if (m_roundHUD->GetPlayerWins() == 2)
	{
		m_showGAME = true;
		std::cout << "Player 2 wins" << std::endl;
		m_roundHUD->resetRounds();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_roundHUD->SetOpponentRoundText(std::to_string((int)m_roundHUD->GetOpponentWins()));
		m_player->SetPosition(800, 400);
		m_serverPlayer->SetPosition(10, 400);
		m_player->ResetHealth();
	}
}
//------------------------------------------------------------------------------------------------------
//function that checks the opponents health
//------------------------------------------------------------------------------------------------------
void PlayState::CheckOpponentHealth()
{
	if (m_opponentHealth == 0 && m_isClientConnected == true)
	{
		std::cout << "Round 1 Over, Player 1 wins" << std::endl;
		m_roundHUD->playerWins();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_serverPlayer->SetPosition(10, 400);
		m_player->SetPosition(800, 400);
		m_player->ResetHealth();
	}

	if (m_roundHUD->GetOpponentWins() == 2)
	{
		m_showGAME = true;
		std::cout << "Player 1 wins" << std::endl;
		m_roundHUD->resetRounds();
		m_roundHUD->SetPlayerRoundText(std::to_string((int)m_roundHUD->GetPlayerWins()));
		m_roundHUD->SetOpponentRoundText(std::to_string((int)m_roundHUD->GetOpponentWins()));
		m_player->SetPosition(800, 400);
		m_serverPlayer->SetPosition(10, 400);
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
	if (m_player->CheckForCollision(m_serverPlayer))
	{
		std::cout << "PLAYER COLLISION" << std::endl;

		//Check if player is punching by running the getter in the player's class
		if (m_player->GetIsPunching() == true)
		{
			std::cout << "PUNCHING COLLISION!!!!" << std::endl;
			m_player->SetHitsLanded(m_player->GetHitsLanded() + 1);
			m_messageSent = "PD_";
			m_messageOut.push(m_messageSent);
			m_player->SetPosition(m_player->GetPosition().x + 30, m_player->GetPosition().y);
			m_serverPlayer->KnockBack();
		}

		//Check if player is kicking by running the getter in the player's class
		if (m_player->GetIsKicking() == true)
		{
			std::cout << "KICKING COLLISION!!!!" << std::endl;
			m_player->SetHitsLanded(m_player->GetHitsLanded() + 1);
			m_messageSent = "KD_";
			m_messageOut.push(m_messageSent);
			m_player->SetPosition(m_player->GetPosition().x + 30, m_player->GetPosition().y);
			m_serverPlayer->KnockBack();
		}		
	}

	if (m_player->CheckForSuperCollision(m_serverPlayer))
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
			m_serverPlayer->KnockBack();
		}
	}
}
//------------------------------------------------------------------------------------------------------
//function for the player input
//------------------------------------------------------------------------------------------------------
void PlayState::PlayerInput()
{
	//Check if the player presses right or left to move the player right or left
	//If they did, send a message to the server with the token "P" and then the
	//X-Pos and Y-Pos of the player
	if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_LEFT])
	{
		m_isPlayerMoving = true;
		m_messageSent = "P_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "MOVE" << std::endl;
	}
	//Check if the player presses space to make the player jump
	//If they did, send a message to the server with the token "J" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_SPACE])
	{
		m_messageSent = "J_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "JUMP" << std::endl;
	}
	//Check if the player presses Z to make the player kick
	//If they did, send a message to the server with the token "K" and then the
	//X-Pos and Y-Pos of the player
	else if (keys[SDL_SCANCODE_Z] && m_isPlayerMoving == false)
	{
		m_messageSent = "K_" + std::to_string(m_player->GetX()) + "_" + std::to_string(m_player->GetY());
		m_messageOut.push(m_messageSent);
		std::cout << "KICK" << std::endl;
	}
	//Check if the player presses X to make the player punch
	//If they did, send a message to the server with the token "H" and then the
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
	//If they did, send a message to the server with the token "I" and then the
	//X-Pos and Y-Pos of the player
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
	delete m_serverPlayer;
	delete m_endGAMEText;
	delete m_clientMessagesHUD;
	delete m_roundHUD;
	delete m_healthBar;
	delete m_image;
	m_client.ShutDown();
}