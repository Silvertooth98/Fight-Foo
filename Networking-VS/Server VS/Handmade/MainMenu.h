/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This class encapsulates the basic default main menu that is presented to the player in both the
  Menu and End states. The menu can be altered to display any menu choice the user desires and 
  once the game is running, will react to key presses to manoeuvre through the menu. 
  Using SetMenuText(), the menu options can be set accordingly and GetMenuOption() returns the 
  index value of the menu choice, once the user presses the ENTER key whilst in-game. Note that 
  all menu item values are zero-based! The menu can also be refreshed using Reset() so that new
  menu choices can be accepted.

*/

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <string>
#include <vector>
#include "GameObject.h"
#include "Text.h"

class MainMenu : public GameObject
{

public:

	MainMenu();
	virtual ~MainMenu();

public:

	int GetMenuOption();
	void SetMenuText(std::string text);

public:

	virtual void Update();
	virtual bool Draw();

public:

	void Reset();

private:

	int m_menuOptionChoice;
	int m_menuOptionActive;
	std::vector<Text> m_menuText;

};

#endif