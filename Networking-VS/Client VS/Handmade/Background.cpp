#include "AudioManager.h"
#include "Background.h"
#include "TextureManager.h"

//------------------------------------------------------------------------------------------------------
//constructor that loads and links resources, and assigns all default values
//------------------------------------------------------------------------------------------------------
Background::Background(std::string imageFilename, std::string audioFilename)
{

	m_isPlaying = false;

	//load image and audio resources into memory
	TheTexture::Instance()->LoadTextureFromFile(imageFilename, imageFilename);
	TheAudio::Instance()->
	LoadFromFile(audioFilename, AudioManager::MUSIC_AUDIO, audioFilename);

	//link image resource with sprite component
	m_image.SetTexture(imageFilename);
	m_image.SetSpriteDimension(1024, 768);
	m_image.SetTextureDimension(1, 1, 1024, 768);

	//link audio resource with music component
	m_music.SetAudio(audioFilename, Audio::MUSIC_AUDIO);
	m_music.SetVolume(25);

	//store names of resource tags so that we can remove them in the destructor 
	m_imageName = imageFilename;
	m_audioName = audioFilename;
	
}
//------------------------------------------------------------------------------------------------------
//function that renders the background image on screen
//------------------------------------------------------------------------------------------------------
bool Background::Draw()
{

	m_image.Draw();
	return true;

}
//------------------------------------------------------------------------------------------------------
//function that plays the background music
//------------------------------------------------------------------------------------------------------
void Background::PlayMusic()
{

	if (!m_isPlaying)
	{
		m_music.Play(Audio::PLAY_ENDLESS);
		m_isPlaying = true;
	}

}
//------------------------------------------------------------------------------------------------------
//function that stops the background music from playing
//------------------------------------------------------------------------------------------------------
void Background::StopMusic()
{

	m_music.Stop();
	m_isPlaying = false;

}
//------------------------------------------------------------------------------------------------------
//destructor that unloads all resources from memory
//------------------------------------------------------------------------------------------------------
Background::~Background()
{

	TheAudio::Instance()->
	UnloadFromMemory(AudioManager::MUSIC_AUDIO, AudioManager::CUSTOM_AUDIO, m_audioName);
	
	TheTexture::Instance()->
	UnloadFromMemory(TextureManager::TEXTURE_DATA, TextureManager::CUSTOM_DATA, m_imageName);

}