#include <iostream>
#include <time.h>
#include "InputManager.h"
#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"

//------------------------------------------------------------------------------------------------------
//constructor that assigns all default values
//------------------------------------------------------------------------------------------------------
MenuState::MenuState(GameState* state) : GameState(state)
{

	m_menu = nullptr;
	m_image = nullptr;
	
}
//------------------------------------------------------------------------------------------------------
//function that creates a new background screen object and menu
//------------------------------------------------------------------------------------------------------
bool MenuState::OnEnter()
{
	m_menu = new MainMenu;

	m_HUD = new HUD();

	m_menu->SetMenuText("JOIN GAME");
	m_menu->SetMenuText("QUIT GAME");

	m_image = new Background("Assets\\Textures\\MainMenu.png", "Assets\\Music\\Main_Menu_Music.ogg");
	
	//seed the random number generator
	srand((unsigned int)time(0));

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that waits for a key to be pressed before transitioning to a different state
//------------------------------------------------------------------------------------------------------
bool MenuState::Update()
{

	//play the background music associated with the image
	//when the state transitions to the next state stop it
	m_image->PlayMusic();

	//update the main menu to determine which menu choice was made
	m_menu->Update();

	//if player chose to play game, go into main playing state
	if (m_menu->GetMenuOption() == JOIN)
	{
		m_image->StopMusic();
		m_isActive = m_isAlive = false;
		TheGame::Instance()->ChangeState(new PlayState(this));
	}

	//if player chose to exit the game then quit altogether
	if (m_menu->GetMenuOption() == QUIT)
	{
		m_image->StopMusic();
		m_isActive = m_isAlive = false;
	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that renders the background splash image and menu
//------------------------------------------------------------------------------------------------------
bool MenuState::Draw()
{

	m_image->Draw();
	m_menu->Draw();
	m_HUD->Draw();
	
	return true;

}
//------------------------------------------------------------------------------------------------------
//function that removes splash screen background object and menu from memory
//------------------------------------------------------------------------------------------------------
void MenuState::OnExit()
{

	delete m_image;
	delete m_menu;

}