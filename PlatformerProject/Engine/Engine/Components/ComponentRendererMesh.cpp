#include "ComponentRendererMesh.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <sre/Texture.hpp>
#include <stdexcept>
#include "glm/gtx/transform.hpp"

TextureMetadata ComponentRendererMesh::LoadMetadata(const std::string& texturePath) {
    //derive metadata file path
    std::string metadataPath = texturePath + ".meta.json";

    //load and parse the metadata JSON
    std::ifstream metadataFile(metadataPath);
    if (!metadataFile.is_open()) {
        throw std::runtime_error("Failed to open metadata file: " + metadataPath);
    }

    std::stringstream buffer;
    buffer << metadataFile.rdbuf();
    rapidjson::Document doc;
    doc.Parse(buffer.str().c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("Failed to parse metadata file: " + metadataPath);
    }

    //extract metadata
    TextureMetadata metadata;
    metadata.textureSize = glm::vec2(
        doc["textureSize"]["x"].GetFloat(),
        doc["textureSize"]["y"].GetFloat());
    metadata.tileSize = glm::vec2(
        doc["tileSize"]["x"].GetFloat(),
        doc["tileSize"]["y"].GetFloat());
    metadata.tileSizeWithBorder = glm::vec2(
        doc["tileSizeWithBorder"]["x"].GetFloat(),
        doc["tileSizeWithBorder"]["y"].GetFloat());
    metadata.minUV = glm::vec2(
        doc["minUV"]["x"].GetFloat(),
        doc["minUV"]["y"].GetFloat());
    metadata.maxUV = glm::vec2(
        doc["maxUV"]["x"].GetFloat(),
        doc["maxUV"]["y"].GetFloat());

    return metadata;
}

    void ComponentRendererMesh::Init(rapidjson::Value & serializedData) {
    // Get the texture path from JSON
    std::string texturePath = serializedData["texture"].GetString();

    //Load metadata
    auto metadata = LoadMetadata(texturePath);
    textureMetadataMap[texturePath] = metadata;

    //Create the texture
    _texture = sre::Texture::create().withFile(texturePath).build();

    //Use metadata for UV calculation
        metadata = textureMetadataMap[texturePath];
        const std::vector<glm::vec4> uvs = {
        glm::vec4(metadata.minUV.x, metadata.minUV.y, 0, 0),
        glm::vec4(metadata.minUV.x, metadata.maxUV.y, 0, 0),
        glm::vec4(metadata.maxUV.x, metadata.maxUV.y, 0, 0),
        glm::vec4(metadata.maxUV.x, metadata.minUV.y, 0, 0)
        };

}

void ComponentRendererMesh::Update(float deltaTime) {

}

void ComponentRendererMesh::Render(sre::RenderPass& renderPass) {
    auto gameObject = GetGameObject().lock();
    if (!gameObject)
        return;

    renderPass.draw(_mesh, gameObject->transform, _material);
    static auto cube = sre::Mesh::create().withCube(0.5f).build();
    static std::vector<std::shared_ptr<sre::Material> > materials = {
            sre::Shader::getUnlit()->createMaterial(),
            sre::Shader::getUnlit()->createMaterial(),
            sre::Shader::getUnlit()->createMaterial()
    };

    std::vector<glm::vec3> positions = {
            {-1,0,-2},
            { 0,0,-3},
            { 1,0,-4}
    };
    std::vector<sre::Color> colors = {
            {1,0,0,1},
            {0,1,0,1},
            {0,0,1,1},
    };
    for (int i = 0; i < positions.size(); i++) {
        materials[i]->setColor(colors[i]);
        renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
    }
}