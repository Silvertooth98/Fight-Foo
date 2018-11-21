#include <fstream>
#include <iostream>
#include <vector>
#include "AudioManager.h"
#include "Game.h"
#include "InputManager.h"
#include "MenuState.h"
#include "ScreenManager.h"
#include "TextureManager.h"

//------------------------------------------------------------------------------------------------------
//constructor that assigns all default values
//------------------------------------------------------------------------------------------------------
Game::Game()
{

	m_endGame = false;
	m_frequency = 0;
	m_chunkSize = 0;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_isFullScreen = false;
	
}
//------------------------------------------------------------------------------------------------------
//getter function that returns total time passed in milliseconds
//------------------------------------------------------------------------------------------------------
int Game::GetTotalTime()
{

	return SDL_GetTicks();

}
//------------------------------------------------------------------------------------------------------
//getter function that returns time elapsed in milliseconds
//------------------------------------------------------------------------------------------------------
int Game::GetElapsedTime()
{

	return m_elapsedTime;

}
//------------------------------------------------------------------------------------------------------
//function that initializes all sub-systems of the game
//------------------------------------------------------------------------------------------------------
bool Game::Initialize(const std::string& filenameInit)
{
	LoadInitFile(filenameInit);

	//initialise game screen with passed values and return false if error occured
	if (!TheScreen::Instance()->Initialize(m_gameName.c_str(), m_screenWidth, m_screenHeight, m_isFullScreen))
	{
		return false;
	}

	//set the background rendering color
	TheScreen::Instance()->SetClearColor(100, 149, 237);

	//initialize audio sub-system and return false if error occured
	if (!(TheAudio::Instance()->Initialize(m_frequency, m_chunkSize)))
	{
		return false;
	}

	//initialize font sub-system and return false if error occured
	if (!TheTexture::Instance()->Initialize())
	{
		return false;
	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that loads and adds a game state to the front of the queue (for temporary states)
//------------------------------------------------------------------------------------------------------
void Game::AddState(GameState* state)
{

	state->OnEnter();
	m_gameStates.push_front(state);
	
}
//------------------------------------------------------------------------------------------------------
//function that loads and adds a game state to the back of the queue (for new states)
//------------------------------------------------------------------------------------------------------
void Game::ChangeState(GameState* state)
{

	state->OnEnter();
	m_gameStates.push_back(state);
	
}
//------------------------------------------------------------------------------------------------------
//function that runs the main game loop and updates all components
//------------------------------------------------------------------------------------------------------
bool Game::Run()
{

	GameState* state;

	//main game loop!
	while (!m_endGame)
	{

		//current active state is always the front one
		state = m_gameStates.front();

		//update and render all objects while the current state is active
		//each state will flag itself as inactive after which the loop breaks
		while (state->IsActive())
		{

			//save time value to mark the start of the frame
			int startTime = SDL_GetTicks();

			//update screen by clearing SDL frame buffer
			TheScreen::Instance()->Update();

			//update input handling by listening for input events
			TheInput::Instance()->Update();

			//update the currently active state
			state->Update();

			//render the currently active state
			state->Draw();
			
			//swap the frame buffer
			TheScreen::Instance()->Draw();

			//calculate time value passed for one frame call
			//if vsync is on this value should be around 16ms
			m_elapsedTime = SDL_GetTicks() - startTime;
			
		}

		//if game state is also flagged as dead  
		//then completely remove all of its objects
		if (!state->IsAlive())
		{
			RemoveState();
		}

		//the main game loop will run as long there are game states available
		m_endGame = m_gameStates.empty();

	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that closes down all sub-systems of the game
//------------------------------------------------------------------------------------------------------
void Game::ShutDown()
{

	//close down font and audio sub-systems
	TheTexture::Instance()->ShutDown();
	TheAudio::Instance()->ShutDown();

	//close down game screen 
	TheScreen::Instance()->ShutDown();

}
//------------------------------------------------------------------------------------------------------
//function that unloads and removes the front-most game state from the queue
//------------------------------------------------------------------------------------------------------
void Game::RemoveState()
{

	m_gameStates.front()->OnExit();

	delete m_gameStates.front();
	m_gameStates.pop_front();

}
//------------------------------------------------------------------------------------------------------
//function that loads and reads the file which handles the data driving aspect
//------------------------------------------------------------------------------------------------------
bool Game::LoadInitFile(const std::string & filename)
{
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << "PARSE STRING" << std::endl;
	std::cout << std::endl;

	std::fstream gameFile("Data/GameIni.txt", std::ios_base::in);

	std::string line;
	std::vector<std::string> vector;

	if (!gameFile)
	{
		std::cout << "Error opening file" << std::endl;
	}

	else
	{
		while (!gameFile.eof())
		{
			std::getline(gameFile, line);
			std::cout << line << std::endl;

			ParseString(line, vector, "=");

			std::cout << std::endl;
		}
		
		m_gameName = vector[1];
		m_screenWidth = (int)(atoi(vector[3].c_str()));
		m_screenHeight = (int)(atoi(vector[5].c_str()));
		m_isFullScreen = (vector[7] == "TRUE");
		m_frequency = (int)(atoi(vector[9].c_str()));
		m_chunkSize = (int)(atoi(vector[11].c_str()));
	}

	gameFile.close();

	std::cout << "------------------------------------------------" << std::endl;

	return false;
}