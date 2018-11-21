/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This class controls the main game. It is a Singleton so that it can be accessed from anywhere
  in the main code, and because only one game can exist at any one time. At its core, it consists
  of a deque of game states, that can be added and removed at any time from within the main
  client code. The main game loop that controls the entire game and its game states runs from
  within the Run() function.

- The AddState() routine will add a temporary game state to the FRONT of the deque. This is 
  ideal for states such as a pause or instruction state. The previous state remains in the deque 
  and as soon as the temporary state is complete, it is removed again. The ChangeState() function
  will add a state to the BACK of the deque and remove the previous state from the FRONT. This
  is ideal for when game states transition from one to another.
  
- The active state will have its OnEnter(), Update(), Draw() and OnExit() routines called 
  accordingly from within this class. As soon as the deque contains no more states, the game 
  will end.

- This class also keeps track of all time passed. It records elapsed time which is great for 
  frame-independent games, where physics or graphics updates rely on time instead of frame rate.
  The class also keeps track of total time passed since the game first started. The Run() 
  function calculates the elapsed time for each frame.

- The Initialize() and ShutDown() functions start up and close down all the other managers
  respectively.

*/

#ifndef GAME_H
#define GAME_H

#include <deque>
#include <string>
#include "GameState.h"
#include "Singleton.h"
#include "Tools.h"

class Game
{

public:

	friend class Singleton<Game>;

public :

	int GetTotalTime();
	int GetElapsedTime();
	
public:

	bool Initialize(const std::string& filenameInit);

	void AddState(GameState* state);
	void ChangeState(GameState* state);

	bool Run();
	void ShutDown();

private:

	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	
private :
	
	void RemoveState();
	bool LoadInitFile(const std::string& filename);

private:

	bool m_endGame;
	int m_elapsedTime;
	std::deque<GameState*> m_gameStates;

	int m_frequency;
	int m_chunkSize;
	int m_screenWidth;
	int m_screenHeight;
	bool m_isFullScreen;
	std::string m_gameName;

};

typedef Singleton<Game> TheGame;

#endif