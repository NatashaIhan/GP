#pragma once

#include "Engine/Component.h"
#include "Engine/GameObject.h"

class PlatformManager : public MyEngine::Component {
	virtual void Init(rapidjson::Value& serializedData) override;
	virtual void Update(float) override;

private:
	float _playerMaxHeight = 0;
	int platformCounter = 0;
};