#pragma once

#include "Engine/Component.h"

enum PlatformType {
	Platform,
	Wall
};

class ComponentPlatform : public MyEngine::Component {
	static constexpr float _tileSize = 380;

public:
	virtual void Init(rapidjson::Value& serializedData) override;
	bool _bouncy;

private:
	float _size;
	PlatformType _type;
};