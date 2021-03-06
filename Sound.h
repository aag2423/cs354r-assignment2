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
static const char *ball_hit = "./media/sounds/ball_hit_2.wav";
static const char *point_up = "./media/sounds/point_up.wav";
static const char *point_down = "./media/sounds/point_down.wav";
static const char *ball_hit_floor = "./media/sounds/ball_hit_floor.wav";
static const char *player_win = "./media/sounds/player_win.wav";
static const char *player_lose = "./media/sounds/player_lose.wav";
static const char *grunt = "./media/sounds/grunt.wav";

static bool sound_effects = true;

class Sound{
public:
	Sound(void);
	~Sound(void);
	void play_sound(const char*);
	void start_ambient();
	void set_ambient_volume(int chan, int vol);
	void set_effects_volume(int vol);
};

static SDL_AudioSpec s_wanted;
static Mix_Chunk *sound_c; 
static Mix_Chunk *ambient_chunk;
static Mix_Chunk *point_up_c;
static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;
static int channel, a_channel, p_u_chan, effects_vol = 64;
#endif
