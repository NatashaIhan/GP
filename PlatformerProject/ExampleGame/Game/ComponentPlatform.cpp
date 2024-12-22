#include "ComponentPlatform.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentPlatform::Init(rapidjson::Value& serializedData) {
	_size = serializedData["size"].GetFloat();
	_type = static_cast<PlatformType>(serializedData["type"].GetInt());
	_bouncy = serializedData["bouncy"].GetBool();

	float halfSize = _size / 2;
	float halfSizeBody = _tileSize / 2; //What is this? //tileSize is a float constant from header file

	glm::vec2 offset = glm::vec2(0, 0);
	if (_type == PlatformType::Platform)
		offset.x = 1;
	if (_type == PlatformType::Wall)
		offset.y = 1;

	// Get the GameObject and its scale
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	glm::vec3 objectScale = gameObject->GetScale();

	// Apply scale to physics body size
	glm::vec2 sizeBody = glm::vec2(380, 220);
	sizeBody *= glm::vec2(objectScale.x, objectScale.y); // Apply GameObject's scale
	sizeBody /= 2; // Box2D expects half-sizes


	auto body = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>().lock();
	body->CreateBody(b2_kinematicBody, false, sizeBody);

	// sprites
	for (int i = -halfSize; i < halfSize; ++i) {
		auto sprite = GetGameObject().lock()->CreateComponent<ComponentRendererSprite>().lock();
		if (_bouncy)
			sprite->SetSprite("bunny-art", "ground_snow_broken.png");
		else
			sprite->SetSprite("bunny-art", "ground_snow.png");
		glm::vec2 pos = offset * _tileSize * (float)i;
		sprite->SetSpritePosition(pos);
    	}
}