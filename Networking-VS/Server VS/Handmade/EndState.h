/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This game state monitors the final state of the game. Things like credits, end screens, final
  messages, etc are intended to be used here. This particular state consists of a background
  object, which represents the backdrop image and audio played, and a menu to offer the player
  some ending choices. For now the default menu choices are to play again or quit the game for
  good.

*/

#ifndef END_STATE_H
#define END_STATE_H

#include <SDL.h>
#include "Background.h"
#include "GameState.h"
#include "MainMenu.h"
#include "ScoreState.h"

class EndState : public GameState
{

public:

	enum MenuOption { PLAY, HS, QUIT };

public:

	EndState(GameState* state);
	virtual ~EndState() {}

public:

	virtual bool OnEnter();
	virtual bool Update();
	virtual bool Draw();
	virtual void OnExit();

private:

	void GameTime();

	MainMenu* m_menu;
	Background* m_image;
	
};

#endif