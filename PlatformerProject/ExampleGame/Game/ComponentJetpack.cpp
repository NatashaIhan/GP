#include "ComponentJetpack.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentJetpack::Init(rapidjson::Value& serializedData) {
	_size = serializedData["size"].GetFloat();

	//float halfSize = _size / 2;
	//float halfSizeBody = _tileSize / 2; //What is this? //tileSize is a float constant from header file

	// body
	glm::vec2 sizeBody = glm::vec2(_size, _size);
	auto body = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>().lock();
	body->CreateBody(b2_kinematicBody, false, sizeBody);

	// sprites
	// handled elsewhere
	/*auto sprite = GetGameObject().lock()->CreateComponent<ComponentRendererSprite>().lock();
	sprite->SetSprite("platformer-art-deluxe", "1.png");
	glm::vec2 pos = _tileSize * (float)i;
	sprite->SetSpritePosition(pos);*/
	
}