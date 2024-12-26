#pragma once

#include "Engine/Component.h"
#include "SDL_mixer.h"

class ComponentSound : public MyEngine::Component {
	private:
		std::string collisionSoundFile;
	public:
		virtual void Init(rapidjson::Value& serializedData) override;
		std::string GetCollisionSoundFile();

};