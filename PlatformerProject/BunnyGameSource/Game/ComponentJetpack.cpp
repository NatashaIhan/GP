#include "ComponentJetpack.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentJetpack::Init(rapidjson::Value& serializedData) {
	_size = serializedData["size"].GetFloat();

	
	glm::vec2 sizeBody = glm::vec2(_size, _size);
	auto body = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>().lock();
	body->CreateBody(b2_kinematicBody, false, sizeBody);
}