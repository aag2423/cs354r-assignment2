#include "Sound.h"


Sound::Sound(void){
	//init SDL library
    SDL_Init(SDL_INIT_AUDIO);
	
	/*set audio format*/
	s_wanted.freq = 22050;
	s_wanted.format = AUDIO_S16SYS;
	s_wanted.channels = 2; // 1 = mono, 2 = stereo
	s_wanted.samples = 4096; // good low-latency value for callback
	s_wanted.callback = fill_audio;
	s_wanted.userdata = NULL;
	
	/* Open the audio device, forcing the desired format */
	if (Mix_OpenAudio(s_wanted.freq, s_wanted.format, s_wanted.channels, s_wanted.samples) != 0){
		fprintf(stdout, "Couldn't open audio: %s\n", SDL_GetError());
	}
}

Sound::~Sound(void){
	Mix_CloseAudio();
	SDL_Quit();
}


void fill_audio(void *udata, Uint8 *stream, int len)
{
	if (audio_len == 0)
	{
		return;
	}	
	else
	{
		// mix as much data as possible
		len = (len > audio_len ? audio_len : len);
		SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
		audio_pos +=len;
		audio_len -= len;
	}
}

void Sound::play_sound(const char* current)
{
	std::cout << "in play_sound \n";
	sound_c = NULL;
	sound_c = Mix_LoadWAV(current);
	if(sound_c == NULL)
		fprintf(stdout, "Unable to load wav file: %s \n", Mix_GetError());
		
	channel = Mix_PlayChannel(-1, sound_c, -1);
	if(channel == -1)
		fprintf(stdout, "Unable to play wav file: %s \n", Mix_GetError());
}

void Sound::set_ambient_volume(int vol)
{
		Mix_Volume(channel, vol);
}