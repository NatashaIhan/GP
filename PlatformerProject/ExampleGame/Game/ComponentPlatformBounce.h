#pragma once

#include "Engine/Component.h"

class ComponentPlatformBounce : public MyEngine::Component {

	bool _bouncy;

	virtual void Init(rapidjson::Value& serializedData) override;

};