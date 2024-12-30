#pragma once

#include "Engine/Component.h"

class ComponentJetpack : public MyEngine::Component {

public:
	virtual void Init(rapidjson::Value& serializedData) override;

private:
	float _size;
};