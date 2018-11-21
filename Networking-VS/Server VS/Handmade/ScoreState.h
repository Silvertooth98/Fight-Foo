#pragma once

#include "GameState.h"
#include "Background.h"
#include <vector>
#include "Text.h"

class ScoreState : public GameState
{
public:
	ScoreState(GameState* state);
	virtual ~ScoreState() {}

public:

	virtual bool OnEnter();
	virtual bool Update();
	virtual bool Draw();
	virtual void OnExit();

private:

	bool LoadFromFile();

private:

	std::vector<Text*> m_highscores;
	Background* m_image;
};

