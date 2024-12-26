#include "ComponentSound.h"

void ComponentSound::Init(rapidjson::Value& serializedData) {
	collisionSoundFile = serializedData["sound_file"].GetString();
}

std::string ComponentSound::GetCollisionSoundFile() {
	return collisionSoundFile;
}