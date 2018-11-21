/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This is the main starting point for the game demo. The main game state is created here and added
  to the main game manager, from where it is run. Change the screen width and height accordingly,
  and feel free to add more game states.

*/

//include SDL main header file to prevent 
//main lib conflicts in Release mode
#include <SDL.h> 
#include <iostream>
#include <string>
#include "Game.h"
#include "MenuState.h"

//------------------------------------------------------------------------------------------------------
//main function that processes everything  
//------------------------------------------------------------------------------------------------------
int main(int argc, char* args[])
{

	//initialize the game
	if (!(TheGame::Instance()->Initialize("Data/GameIni.txt")))
	{
		return 0;
	}

	//create the first state to be used in the game
	TheGame::Instance()->AddState(new MenuState(nullptr));

	//run the game
	if (!TheGame::Instance()->Run())
	{
		return 0;
	}

	//close down the game
	TheGame::Instance()->ShutDown();

	//end application
	return 0;

}