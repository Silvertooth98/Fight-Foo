/*

  All code has been written by Karsten Vermeulen and may be used freely by anyone. It is by no
  means perfect and there is certainly room for improvement in some parts. As a whole, the code
  has been created as part of an engine for the purposes of educating other fellow programmers,
  and anyone else wishing to learn C++ and OOP. Feel free to use, copy, break, update and do as
  you wish with this code - it is there for all!

  UPDATED : July 2017

  -----------------------------------------------------------------------------------------------

- This class is designed to represent the background image and music for each game state. Therefore
  it takes in image and audio filename arguments to correctly load and use the right resources.
  Ideally, this class should be instantiated within each game state as there will be different 
  backdrops and music playing in each one.

*/

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Audio.h"
#include "GameObject.h"
#include "Sprite.h"

class Background : public GameObject
{

public:

	Background(std::string imageFilename, std::string audioFilename);
	virtual ~Background();

public:

	virtual void Update() {}
	virtual bool Draw();

public:

	void PlayMusic();
	void StopMusic();

protected:

	bool m_isPlaying;
	std::string m_imageName;
	std::string m_audioName;

	Audio m_music;
	Sprite m_image;

};

#endif
