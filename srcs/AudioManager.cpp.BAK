#include "AudioManager.hpp"
#include "ResourceManager.hpp"
#include "NibblerException.hpp"

AudioManager &	AudioManager::getInstance(void)
{
	static AudioManager instance;

	return (instance);
}

AudioManager::AudioManager(void)
{
	if (SDL_Init(SDL_INIT_AUDIO))
		throw NibblerException("SDL_Init() failed");

	if ((Mix_Init(MIX_FLAGS) & MIX_FLAGS) != MIX_FLAGS)
		throw NibblerException("Mix_Init() failed");

	if (Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0)
		throw NibblerException("Mix_OpenAudio() failed");

	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

	this->_loadMusic(&this->_bgm[0], BGM_00);
	this->_loadMusic(&this->_bgm[1], BGM_01);
	this->_loadMusic(&this->_bgm[2], BGM_02);
	this->_loadMusic(&this->_bgm[3], BGM_03);
	this->_loadMusic(&this->_bgm[4], BGM_04);
	this->_loadMusic(&this->_bgm[5], BGM_05);
	this->_loadMusic(&this->_bgm[6], BGM_06);
	this->_loadMusic(&this->_bgm[7], BGM_07);
	this->_loadMusic(&this->_bgm[8], BGM_08);
	this->_loadMusic(&this->_bgm[9], BGM_09);
	this->_loadMusic(&this->_bgm[10], BGM_10);
	this->_loadMusic(&this->_bgm[11], BGM_11);
	this->_loadMusic(&this->_bgm[12], BGM_12);
	this->_loadMusic(&this->_bgm[13], BGM_13);
	this->_loadMusic(&this->_bgm[14], BGM_14);
	this->_loadMusic(&this->_bgm[15], BGM_15);

	this->_loadWAV(&this->_eatingSFX, EATING_SFX);
	this->_loadWAV(&this->_deathSFX, DEATH_SFX);
	this->_loadWAV(&this->_hurtSFX, HURT_SFX);

	this->_loadWAV(&this->_1PWinSFX, _1P_WIN_SFX);
	this->_loadWAV(&this->_1PLoseSFX, _1P_LOSE_SFX);

	this->_loadWAV(&this->_2PP1WinSFX, _2P_P1_WIN_SFX);
	this->_loadWAV(&this->_2PP2WinSFX, _2P_P2_WIN_SFX);
	this->_loadWAV(&this->_2PAllLoseSFX, _2P_ALL_LOSE_SFX);
}

void		AudioManager::_loadMusic(Mix_Music ** target, std::string filename)
{
	*target = Mix_LoadMUS(ResourceManager::getInstance().getAbsolutePathname(filename).c_str());

	if (!*target)
		throw NibblerException("Mix_LoadMUS() failed on \'" + filename + "\'");
}

void		AudioManager::_loadWAV(Mix_Chunk ** target, std::string filename)
{
	*target = Mix_LoadWAV(ResourceManager::getInstance().getAbsolutePathname(filename).c_str());
	
	if (!*target)
		throw NibblerException("Mix_LoadWAV() failed on \'" + filename + "\'");
}

AudioManager::~AudioManager(void)
{
	for (int i = 0; i < BGM_COUNT; i++)
		Mix_FreeMusic(this->_bgm[i]);
	Mix_FreeChunk(this->_eatingSFX);
	Mix_FreeChunk(this->_deathSFX);
	Mix_Quit();
	SDL_Quit();
}

void			AudioManager::playBGM(void)
{
	if (Mix_PlayingMusic())
		Mix_HaltMusic();

	Mix_PlayMusic(this->_bgm[std::rand() % BGM_COUNT], -1);		// loop forever
}

void			AudioManager::playEatingSFX(void)
{
	Mix_PlayChannel(-1, this->_eatingSFX, 0);
}

void			AudioManager::playDeathSFX(void)
{
	Mix_PlayChannel(-1, this->_deathSFX, 0);
}

void			AudioManager::playHurtSFX(void)
{
	Mix_PlayChannel(-1, this->_hurtSFX, 0);
}

void			AudioManager::announce1PWin(void)
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, this->_1PWinSFX, 0);
}

void			AudioManager::announce1PLose(void)
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, this->_1PLoseSFX, 0);
}

void			AudioManager::announce2PP1Win(void)
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, this->_2PP1WinSFX, 0);
}

void			AudioManager::announce2PP2Win(void)
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, this->_2PP2WinSFX, 0);
}

void			AudioManager::announce2PAllLose(void)
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, this->_2PAllLoseSFX, 0);
}











