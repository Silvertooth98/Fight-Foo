/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This class encapsulates a game state that will exist within the game. A game state can be
  anything like a play state, pause state, menu state, etc and will contain all the game
  specific code specific to that state. Every game state needs to be instantiated as a
  sub-class of this abstract base class.

- The main flag variable m_isActive will keep track if the game state is currently active or not.
  As long as its active, its overriden Update() and Draw() routines will constantly be called until
  the state is set as not active, at which point the state will close down.

- The OnEnter() routine is called when the state is activated and this function will load all
  resources from file, instantiate the game objects for that state and do all start up tasks for the
  active state. The opposing OnExit() function will do the opposite when the state is deactivated,
  ie - call all shutdown tasks, remove all game objects from memory and free all resources from
  memory for the given state.

*/

#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{

public:
	
	GameState(GameState* state);
	virtual ~GameState() = 0 {}

public:

	bool& IsAlive();
	bool& IsActive();

public:

	virtual bool OnEnter() = 0;
	virtual bool Update() = 0;
	virtual bool Draw() = 0;
	virtual void OnExit() = 0;

protected:

	bool m_isAlive;
	bool m_isActive;
	GameState* m_previousState;

};

#endif