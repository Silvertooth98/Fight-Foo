#include "ScoreState.h"
#include "InputManager.h"

ScoreState::ScoreState(GameState* state) : GameState(state)
{
	m_image = nullptr;
}

bool ScoreState::OnEnter()
{
	m_image = new Background("Assets\\Textures\\ScoreScreen.png", "Assets\\Audio\\End.ogg");
	return true;
}

bool ScoreState::Update()
{
	const Uint8* keys = TheInput::Instance()->GetKeyStates();

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		m_isActive = m_isAlive = false;
		m_previousState->IsActive() = true;
	}

	if (keys[SDL_SCANCODE_C])
	{
		//Reset the highscores
	}

	return true;
}

bool ScoreState::Draw()
{
	m_image->Draw();

	return false;
}

void ScoreState::OnExit()
{
	//shutdown tasks here

	delete m_image;
}

bool ScoreState::LoadFromFile()
{
	//Load the highscores here

	return false;
}
