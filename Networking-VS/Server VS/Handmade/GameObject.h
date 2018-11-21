/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This class encapsulates a game object that will exist in the game world and represent any type
  of object like a player, enemy, weapon, inventory item, etc. It is designed around the same
  principle as Unity's game objects and will be instantiated in the client code at runtime from
  within the game specific code. It consists of three main flags, m_isAlive, m_isActive and
  m_isVisible, to signify if the object is exisiting in memory, being used in the game world and
  hidden or not from the game world, respectively. Each game object also has a string tag to
  identify it amongst other objects, and a priority value for 2D game worlds without a Z-axis,
  so that game objects may be drawn in a specific order to give the illusion of depth. All game 
  objects need to be instantiated as sub-classes of this abstract base class.

- The getters and setters are split into normal ones and combo variants. The combo variants,
  IsAlive(), IsActive() and IsVisible() return references to the respective variables because these
  flag variables may be used frequently and getting and setting them individually may become
  inconvenient.

- Each game object has a Update() and Draw() function which will be overriden in the sub-class,
  where the actual main functionality of that object will exist. Generally the Update() routine
  is there for the updating of all position, rotation and scale values, alongside others. The Draw()
  function is intended to render all internal components and other objects.

*/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <glm.hpp>

class GameObject 
{

public :

	GameObject();
	virtual ~GameObject() = 0 {}
	
public:

	bool& IsAlive();
	bool& IsActive();
	bool& IsVisible();

public :

	std::string GetTag();
	glm::vec2 GetPosition();

	void SetTag(std::string tag);
	void SetPosition(float x, float y);

public :

	virtual void Update() = 0;
	virtual bool Draw() = 0;

protected :

	bool m_isAlive;
	bool m_isActive;
	bool m_isVisible;

	std::string m_tag;
	glm::vec2 m_position;

};

#endif