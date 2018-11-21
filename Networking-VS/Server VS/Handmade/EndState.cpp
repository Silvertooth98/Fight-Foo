#include <fstream>
#include <iostream>
#include "EndState.h"
#include "Game.h"
#include "InputManager.h"
#include "PlayState.h"

//------------------------------------------------------------------------------------------------------
//constructor that assigns all default values
//------------------------------------------------------------------------------------------------------
EndState::EndState(GameState* state) : GameState(state)
{

	m_image = nullptr;

}
//------------------------------------------------------------------------------------------------------
//function that creates a new background screen object and menu
//------------------------------------------------------------------------------------------------------
bool EndState::OnEnter()
{

	m_menu = new MainMenu;
	m_menu->SetMenuText("TRY AGAIN");
	m_menu->SetMenuText("HIGHSCORES");
	m_menu->SetMenuText("QUIT GAME");

	m_image = new Background("Assets\\Textures\\EndScreen.png", "Assets\\Audio\\End.ogg");

	return true;

}
//------------------------------------------------------------------------------------------------------
//function for file handling 
//------------------------------------------------------------------------------------------------------
void EndState::GameTime()
{
	std::fstream myFile("Data/TimeFile.dat", std::ios_base::in);
	std::fstream AddToFile("Data/TimeFile.dat", std::ios_base::app);

	if (!myFile)
	{
		std::fstream openFile("Data/TimeFile.dat", std::ios_base::out);
		openFile << "Server Game Times:" << std::endl;
		openFile << "-----------------" << std::endl;

		std::cout << "Writing to the file" << std::endl;

		AddToFile << "\nTotal time in game: ";
		AddToFile << TheGame::Instance()->GetTotalTime() / 1000 << " Seconds";
	}

	if (myFile)
	{
		std::cout << "Writing to the file" << std::endl;

		AddToFile << "\nTotal time in game: ";
		AddToFile << TheGame::Instance()->GetTotalTime() / 1000 << " Seconds";
	}

	myFile.close();
}
//------------------------------------------------------------------------------------------------------
//function that waits for a Q key to be pressed before ending all game states (the end!)
//------------------------------------------------------------------------------------------------------
bool EndState::Update()
{

	//play the background music associated with the image
	//when the state transitions to the next state stop it
	m_image->PlayMusic();

	//update the main menu to determine which menu choice was made
	m_menu->Update();

	//if player wants to play again then return to the main playing state
	if (m_menu->GetMenuOption() == PLAY)
	{
		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		TheGame::Instance()->ChangeState(new PlayState(this));
	}

	//if player wants to view highscores
	if (m_menu->GetMenuOption() == HS)
	{
		m_menu->Reset();
		m_isActive = false;
		TheGame::Instance()->AddState(new ScoreState(this));
	}

	//if player chose to exit the game then quit altogether
	if (m_menu->GetMenuOption() == QUIT)
	{
		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		GameTime();
	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that renders the background splash image
//------------------------------------------------------------------------------------------------------
bool EndState::Draw()
{

	m_image->Draw();
	m_menu->Draw();

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that removes splash screen background object from memory
//------------------------------------------------------------------------------------------------------
void EndState::OnExit()
{

	delete m_image;
	//delete m_menu;

}