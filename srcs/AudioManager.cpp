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

	this->_bgmClips.push_back(this->_loadBGM(BGM_00));
	this->_bgmClips.push_back(this->_loadBGM(BGM_01));
	this->_bgmClips.push_back(this->_loadBGM(BGM_02));
	this->_bgmClips.push_back(this->_loadBGM(BGM_03));
	this->_bgmClips.push_back(this->_loadBGM(BGM_04));
	this->_bgmClips.push_back(this->_loadBGM(BGM_05));
	this->_bgmClips.push_back(this->_loadBGM(BGM_06));
	this->_bgmClips.push_back(this->_loadBGM(BGM_07));
	this->_bgmClips.push_back(this->_loadBGM(BGM_08));
	this->_bgmClips.push_back(this->_loadBGM(BGM_09));

	this->_sfxClips["eating"] = this->_loadSFX(EATING_SFX);
	this->_sfxClips["death"] = this->_loadSFX(DEATH_SFX);
	this->_sfxClips["hurt"] = this->_loadSFX(HURT_SFX);

	this->_sfxClips["1p_win"] = this->_loadSFX(_1P_WIN_SFX);
	this->_sfxClips["1p_lose"] = this->_loadSFX(_1P_LOSE_SFX);

	this->_sfxClips["2p_p1_win"] = this->_loadSFX(_2P_P1_WIN_SFX);
	this->_sfxClips["2p_p2_win"] = this->_loadSFX(_2P_P2_WIN_SFX);
	this->_sfxClips["2p_all_lose"] = this->_loadSFX(_2P_ALL_LOSE_SFX);
}

Mix_Music *		AudioManager::_loadBGM(std::string filename)
{
	Mix_Music *	clip = Mix_LoadMUS(ResourceManager::getInstance().getBasePath(filename).c_str());

	if (!clip)
		throw NibblerException("Mix_LoadMUS() failed on \'" + filename + "\'");
	return (clip);
}

Mix_Chunk *		AudioManager::_loadSFX(std::string filename)
{
	Mix_Chunk *	clip = Mix_LoadWAV(ResourceManager::getInstance().getBasePath(filename).c_str());
	
	if (!clip)
		throw NibblerException("Mix_LoadWAV() failed on \'" + filename + "\'");
	return (clip);
}

AudioManager::~AudioManager(void)
{
	for (size_t i = 0; i < this->_bgmClips.size(); i++)
		Mix_FreeMusic(this->_bgmClips[i]);
	for (const auto & clip : this->_sfxClips)
		Mix_FreeChunk(clip.second);
	Mix_Quit();
	SDL_Quit();
}

void			AudioManager::playBGM(void)
{
	Mix_Music *	clip = this->_bgmClips[std::rand() % this->_bgmClips.size()];

	Mix_PlayMusic(clip, -1);		// loop forever
}

void			AudioManager::playSFX(std::string id)
{
	Mix_Chunk *	clip = this->_sfxClips[id];

	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, clip, 0);
}
