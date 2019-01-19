#ifndef AUDIO_MANAGER_HPP
# define AUDIO_MANAGER_HPP

# include <string>
# include <SDL.h>
# include <SDL_mixer.h>

# define MIX_FLAGS			(MIX_INIT_MP3)

# define BGM_COUNT			16
# define BGM_00				"bgm/ANewMorning.mp3"
# define BGM_01				"bgm/Crystals.mp3"
# define BGM_02				"bgm/Daisuke.mp3"
# define BGM_03				"bgm/DeepCover.mp3"
# define BGM_04				"bgm/ElectricDreams.mp3"
# define BGM_05				"bgm/Flatline.mp3"
# define BGM_06				"bgm/Hotline.mp3"
# define BGM_07				"bgm/Hydrogen.mp3"
# define BGM_08				"bgm/InnerAnimal.mp3"
# define BGM_09				"bgm/ItsSafeNow.mp3"
# define BGM_10				"bgm/Knock.mp3"
# define BGM_11				"bgm/Miami2.mp3"
# define BGM_12				"bgm/Musikk2.mp3"
# define BGM_13				"bgm/Paris2.mp3"
# define BGM_14				"bgm/Perturbator.mp3"
# define BGM_15				"bgm/TurfMain.mp3"

# define EATING_SFX			"sounds/om_nom.wav"
# define DEATH_SFX			"sounds/wilheim_scream.wav"
# define HURT_SFX			"sounds/minecraft_ugh.wav"

# define _1P_WIN_SFX		"say/1P_win.wav"
# define _1P_LOSE_SFX		"say/1P_lose.wav"

# define _2P_P1_WIN_SFX		"say/2P_P1_win.wav"
# define _2P_P2_WIN_SFX		"say/2P_P2_win.wav"
# define _2P_ALL_LOSE_SFX	"say/2P_all_lose.wav"

class AudioManager
{
public:

	~AudioManager(void);

	static AudioManager &	getInstance(void);

	void					playBGM(void);
	void					playEatingSFX(void);
	void					playDeathSFX(void);
	void					playHurtSFX(void);

	void					announce1PWin(void);
	void					announce1PLose(void);

	void					announce2PP1Win(void);
	void					announce2PP2Win(void);
	void					announce2PAllLose(void);

private:

	Mix_Music *				_bgm[BGM_COUNT];

	Mix_Chunk *				_eatingSFX;
	Mix_Chunk *				_deathSFX;
	Mix_Chunk *				_hurtSFX;

	Mix_Chunk *				_1PWinSFX;
	Mix_Chunk *				_1PLoseSFX;

	Mix_Chunk *				_2PP1WinSFX;
	Mix_Chunk *				_2PP2WinSFX;
	Mix_Chunk *				_2PAllLoseSFX;

	AudioManager(void);
	AudioManager(const AudioManager & src);
	AudioManager & operator=(const AudioManager & rhs);

	void					_loadMusic(Mix_Music ** target, std::string filename);
	void					_loadWAV(Mix_Chunk ** target, std::string filename);

};

#endif
