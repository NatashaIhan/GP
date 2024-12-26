#include <iostream>
#include "ComponentSoundSystem.h"


void ComponentSoundSystem::Init(rapidjson::Value& serializedData) {
	int channels = serializedData["channels"].GetInt();
	int init = Mix_OpenAudio(
		22050,
		MIX_DEFAULT_FORMAT,
		channels,
		2048
	);
	LoadMusic();
	PlayMusic();
}

void ComponentSoundSystem::LoadMusic() {
	music_file = Mix_LoadMUS("data/PackedDensityTime.mp3");
	if (!music_file) {
		std::cout << "Music Error: " << Mix_GetError();
	}
}

void ComponentSoundSystem::PlayMusic() {
	int playing = Mix_PlayMusic(music_file, 0);
	if (!music_file) {
		std::cout << "Music Error: " << Mix_GetError();
	}
	std::cout << "Playing music";
}

void ComponentSoundSystem::PlaySFX(std::string sound_file_name) {
	Mix_Chunk* sound_file = Mix_LoadWAV(sound_file_name.c_str());
	Mix_PlayChannel(
		-1, // int channel to play on (-1 is first available)
		sound_file, // Mix_Chunk* chunk to play
		0 // int number loops
	);
}
