/*
Anthony Garza 
Sound.h
created: 4/17/2014
*/

#ifndef __Sound_h_
#define __Sound_h_


#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

//CONSTANTS TO IDENTIFY EACH SOUND
static const char *ambient_sound = "./media/sounds/spring-weather-1.wav";
static const char *ball_hit = "./media/sounds/ball_hit.wav";
static const char *point_up = "./media/sounds/point_up.wav";

class Sound{
public:
	Sound(void);
	~Sound(void);
	void play_sound(const char*);
};

static SDL_AudioSpec s_wanted;
static Mix_Chunk *sound_c; 
static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;
static int channel;
void fill_audio(void *udata, Uint8 *stream, int len);
#endif
