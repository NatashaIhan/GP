#include "ComponentPlatform.h"

#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"

void ComponentPlatform::Init(rapidjson::Value& serializedData) {
	_size = serializedData["size"].GetFloat();
	_type = static_cast<PlatformType>(serializedData["type"].GetInt());

	float halfSize = _size / 10;
	float halfSizeBody = _tileSize / 10; //What is this? //tileSize is a float constant from header file

	// Apply scaling if scale property exists
	if (serializedData.HasMember("transform") && serializedData["transform"].HasMember("scale")) {
		auto scaleData = serializedData["transform"]["scale"].GetArray();
		glm::vec3 scale(scaleData[0].GetFloat(), scaleData[1].GetFloat(), scaleData[2].GetFloat());
		GetGameObject().lock()->SetScale(scale);
	}

	glm::vec2 offset = glm::vec2(0, 0);
	if (_type == PlatformType::Platform)
		offset.x = 1;
	if (_type == PlatformType::Wall)
		offset.y = 1;
	
	// body
	glm::vec2 sizeBody = glm::vec2(_tileSize, _tileSize) + offset * _tileSize * (_size - 1);
	sizeBody /= 10;
	auto body = GetGameObject().lock()->CreateComponent<ComponentPhysicsBody>().lock();
	body->CreateBody(b2_kinematicBody, false, sizeBody);

	// sprites
	for (int i = -halfSize; i < halfSize; ++i) {
		auto sprite = GetGameObject().lock()->CreateComponent<ComponentRendererSprite>().lock();
		sprite->SetSprite("bunny-art", "ground_snow.png");
		glm::vec2 pos = offset * _tileSize * (float)i;
		sprite->SetSpritePosition(pos);
	}
}