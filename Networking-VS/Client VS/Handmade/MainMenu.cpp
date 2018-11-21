#include "InputManager.h"
#include "MainMenu.h"
#include "TextureManager.h"

//------------------------------------------------------------------------------------------------------
//constructor that loads and links resources, and sets up the main menu text defaults
//------------------------------------------------------------------------------------------------------
MainMenu::MainMenu()
{

	//set default menu choices
	m_menuOptionActive = 0;
	m_menuOptionChoice = -1;

	//load font resource into memory
	TheTexture::Instance()->LoadFontFromFile("Assets\\Fonts\\Denka.ttf", 100, "MENU_FONT");

}
//------------------------------------------------------------------------------------------------------
//getter function that returns main menu choice made
//------------------------------------------------------------------------------------------------------
int MainMenu::GetMenuOption()
{

	return m_menuOptionChoice;

}
//------------------------------------------------------------------------------------------------------
//setter function that assigns properties of menu text objects
//------------------------------------------------------------------------------------------------------
void MainMenu::SetMenuText(std::string text)
{

	Text menuText;
	
	menuText.SetFont("MENU_FONT");
	menuText.SetColor(255, 174, 0);
	menuText.SetSize(text.size() * 20, 60);
	menuText.SetText(text);
	m_menuText.push_back(menuText);

}
//------------------------------------------------------------------------------------------------------
//function that updates the main menu text on screen
//------------------------------------------------------------------------------------------------------
void MainMenu::Update()
{

	const Uint8* keys = TheInput::Instance()->GetKeyStates();

	//flag to monitor if key is pressed so that when pressing UP/DOWN
	//key the selected menu option doesn't move at lightning speed
	static bool isKeyPressed = false;

	//only if DOWN arrow key is pressed and key was not pressed before move 1 option down the menu
	//also check if the last menu item has been reached, which means we need to loop back to the top
	if (keys[SDL_SCANCODE_DOWN] && !isKeyPressed)
	{
		
		if (m_menuOptionActive == m_menuText.size() - 1)
		{
			m_menuOptionActive = 0;
		}
		
		else
		{
			m_menuOptionActive++;
		}

	}

	//only if UP arrow key is pressed and key was not pressed before move 1 option up the menu
	//also check if the first menu item has been reached, which means we need to go to the bottom
	if (keys[SDL_SCANCODE_UP] && !isKeyPressed)
	{

		if (m_menuOptionActive == 0)
		{
			m_menuOptionActive = m_menuText.size() - 1;
		}

		else
		{
			m_menuOptionActive--;
		}

	}

	//if ENTER key is pressed assign menu item as choice made 
	if (keys[SDL_SCANCODE_RETURN])
	{
		m_menuOptionChoice = m_menuOptionActive;
	}

	//update state of key based on if it's pressed or not which will make sure the next time
	//the frame is called the above code will either move the menu option or keep it still
	isKeyPressed = TheInput::Instance()->IsKeyPressed();

	//loop through all menu items and set their initial color to orange
	for (size_t i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].SetColor(255, 174, 0);
	}

	//only set the active menu item to a red color
	m_menuText[m_menuOptionActive].SetColor(240, 0, 0);

}
//------------------------------------------------------------------------------------------------------
//function that renders the main menu text on screen
//------------------------------------------------------------------------------------------------------
bool MainMenu::Draw()
{

	//calculate a centre position for all menu items based on amount of menu items and their height
	//this will keep all menu items centered around the same spot no matter how many items there are
	int yPos = (int)(620.0f - (float(m_menuText.size()) / 2.0f * 60.0f));

	//loop through all menu items and render them based on a centre position
	for (size_t i = 0; i < m_menuText.size(); i++)
	{
		m_menuText[i].Draw((int)(512 - m_menuText[i].GetSize().x / 2), (int)(yPos + i * 60));
	}

	return true;

}
//------------------------------------------------------------------------------------------------------
//function that resets the menu back to its default
//------------------------------------------------------------------------------------------------------
void MainMenu::Reset()
{

	m_menuOptionActive = 0;
	m_menuOptionChoice = -1;

}
//------------------------------------------------------------------------------------------------------
//destructor that unloads all resources from memory
//------------------------------------------------------------------------------------------------------
MainMenu::~MainMenu()
{

	TheTexture::Instance()->
	UnloadFromMemory(TextureManager::FONT_DATA, TextureManager::CUSTOM_DATA, "MENU_FONT");

}