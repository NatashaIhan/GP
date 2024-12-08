#pragma once

#include "Engine/Component.h"
#include <string>
#include <glm/vec2.hpp>
#include <rapidjson/document.h>
#include <unordered_map>

struct TextureMetadata {
	glm::vec2 textureSize;
	glm::vec2 tileSize;
	glm::vec2 tileSizeWithBorder;
	glm::vec2 minUV;
	glm::vec2 maxUV;
};

class ComponentRendererMesh : public MyEngine::Component {
public:
	void Init(rapidjson::Value& serializedData);
	void Update(float deltaTime) override;
	void Render(sre::RenderPass& renderPass) override;

private:
	std::shared_ptr<sre::Mesh> _mesh;
	std::shared_ptr<sre::Material> _material;
	std::shared_ptr<sre::Texture> _texture;

	// Texture metadata
	std::unordered_map<std::string, TextureMetadata> textureMetadataMap;

	//Helper function to load texture metadata
	TextureMetadata LoadMetadata(const std::string& textureName);
};