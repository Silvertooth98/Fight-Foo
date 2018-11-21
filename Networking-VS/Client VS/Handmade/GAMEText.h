#pragma once

#include "GameObject.h"
#include "Sprite.h"

class GAMEText : public GameObject
{
public:
	GAMEText();
	virtual ~GAMEText();

	virtual void Update() {}
	virtual bool Draw();

private:

	Sprite m_image;
};

