#include "GameObject.h"

//------------------------------------------------------------------------------------------------------
//constructor that assigns all defaults 
//------------------------------------------------------------------------------------------------------
GameObject::GameObject()
{

	m_isAlive = true;
	m_isActive = true;
	m_isVisible = true;

	m_tag = "";
	
}
//------------------------------------------------------------------------------------------------------
//getter-setter function that returns memory flag reference
//------------------------------------------------------------------------------------------------------
bool& GameObject::IsAlive()
{

	return m_isAlive;

}
//------------------------------------------------------------------------------------------------------
//getter-setter function that returns active flag reference
//------------------------------------------------------------------------------------------------------
bool& GameObject::IsActive()
{

	return m_isActive;

}
//------------------------------------------------------------------------------------------------------
//getter-setter function that returns visibility flag reference
//------------------------------------------------------------------------------------------------------
bool& GameObject::IsVisible()
{

	return m_isVisible;

}
//------------------------------------------------------------------------------------------------------
//getter function that returns game object's tag variable
//------------------------------------------------------------------------------------------------------
std::string GameObject::GetTag()
{

	return m_tag;

}
//------------------------------------------------------------------------------------------------------
//getter function that returns game object's position on screen
//------------------------------------------------------------------------------------------------------
glm::vec2 GameObject::GetPosition()
{

	return m_position;

}
//------------------------------------------------------------------------------------------------------
//setter function that assigns game object's tag variable
//------------------------------------------------------------------------------------------------------
void GameObject::SetTag(std::string tag)
{

	m_tag = tag;

}
//------------------------------------------------------------------------------------------------------
//setter function that assigns game object's position on screen
//------------------------------------------------------------------------------------------------------
void GameObject::SetPosition(float x, float y)
{

	m_position = glm::vec2(x, y);

}