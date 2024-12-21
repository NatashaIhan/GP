#include "ComponentPlatformBounce.h"

void ComponentPlatformBounce::Init(rapidjson::Value& serializedData) {
	auto gameObject = GetGameObject().lock();
	if (!gameObject)
		return;

	_bouncy = serializedData["bouncy"].GetBool();
}