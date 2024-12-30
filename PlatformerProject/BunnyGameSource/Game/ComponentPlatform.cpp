#include "ComponentPlatform.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentPlatform::Init(rapidjson::Value& serializedData) {
	porous = serializedData["bouncy"].GetBool();

	// Get the GameObject and its scale
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	glm::vec3 objectScale = gameObject->GetScale();

	// Apply scale to physics body size
	glm::vec2 sizeBody = _platformSize * glm::vec2(objectScale.x, objectScale.y);// Apply GameObject's scale
	sizeBody /= 2; // Box2D expects half-sizes

	auto body = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>().lock();
	body->CreateBody(b2_kinematicBody, false, sizeBody);

	// sprites
	auto sprite = GetGameObject().lock()->CreateComponent<ComponentRendererSprite>().lock();
	if (porous) {
		sprite->SetSprite("bunny-art", "ground_snow_broken.png");
	}
	else {
		sprite->SetSprite("bunny-art", "ground_snow.png");
	}
}
	
