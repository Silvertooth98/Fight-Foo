#pragma once

#include "GameObject.h"
#include "Sprite.h"

class GAMEtext : public GameObject
{
public:
	GAMEtext();
	virtual ~GAMEtext();

	virtual void Update() {}
	virtual bool Draw();
	
private:

	Sprite m_image;
};