#include "PlatformManager.h"
#include "Engine/MyEngine.h"
#include "Engine/ComponentFactory.h"
#include "rapidjson/document.h"
#include <sstream>

void PlatformManager::Init(rapidjson::Value& serializedData) {
}

void PlatformManager::Update(float deltaTime) {
    auto player = GetGameObject().lock();
    if (!player) return;

    float currentHeight = player->GetPosition().y;
    int platformSpaceing = 200;
    if ((int)currentHeight / platformSpaceing > (int)_playerMaxHeight / platformSpaceing) {
        // Player has reached a new height
        _playerMaxHeight = currentHeight;
        auto platform = MyEngine::Engine::GetInstance()->CreateGameObject("NewPlatform" + std::to_string(platformCounter));
        platformCounter++;
        auto platformPtr = platform.lock();
        if (!platformPtr) return;

        glm::vec2 screenSize = MyEngine::Engine::GetInstance()->GetScreenSize();
        int platformWidth = 380;
        int halfWidth = platformWidth / 2;

        // Fix spawn position calculation
        int minX = -183;  // Match the left boundary where player wraps
        int maxX = 497;   // Match the right boundary where player wraps
        int spawnPositionX = rand() % (maxX - minX + 1) + minX;
        int spawnPositionY = _playerMaxHeight + (int)screenSize.y / 2 + 100;

        bool isBouncy = rand() % 100 < 35;
        bool spawnJetpack = rand() % 100 < 15;  // 15% chance to spawn jetpack

        // Spawn jetpack if rolled
        if (spawnJetpack) {
            auto jetpack = MyEngine::Engine::GetInstance()->CreateGameObject("Jetpack" + std::to_string(platformCounter));
            auto jetpackPtr = jetpack.lock();
            if (jetpackPtr) {
                rapidjson::Document jetpackParams;
                std::stringstream jp;
                jp << "{";
                jp << "\"name\": \"Jetpack" << std::to_string(platformCounter) << "\",";
                jp << "\"transform\" : {";
                jp << "\"position\":[ " << spawnPositionX << ", " << (spawnPositionY + 60) << ", 0], ";
                jp << "\"rotation\" : [0, 0, 0] ,";
                jp << "\"scale\" : [0.5, 0.5, 0.5]";
                jp << "},";
                jp << "\"components\" : [";
                jp << "{";
                jp << "\"typeId\": \"SPRITE_RENDERER\",";
                jp << "\"serializedData\" : {";
                jp << "\"atlas\": \"bunny-art\",";
                jp << "\"sprite\": \"jetpack.png\"";
                jp << "}";
                jp << "},";
                jp << "{";
                jp << "\"typeId\": \"JETPACK\",";
                jp << "\"serializedData\" : {";
                jp << "\"size\": 8";
                jp << "}";
                jp << "},";
                jp << "{";
                jp << "\"typeId\": \"COLLISION_SOUND\",";
                jp << "\"serializedData\" : {";
                jp << "\"sound_file\": \"data/750234__universfield__blaster-shot.mp3\"";
                jp << "}";
                jp << "}";
                jp << "]";
                jp << "}";
                jetpackParams.Parse(jp.str().c_str());
                jetpackPtr->Init(jetpackParams);
            }
        }

        // Platform spawn parameters
        rapidjson::Document platformSpawnParameters;
        std::stringstream ss;
        ss << "{";
        ss << "\"name\": \"NewPlatform" << std::to_string(platformCounter) << "\",";
        ss << "\"transform\" : {";
        ss << "\"position\":[ " << spawnPositionX << ", " << spawnPositionY << ", 0], ";
        ss << "\"rotation\" : [0, 0, 0] ,";
        ss << "\"scale\" : [0.5, 0.5, 0.5]";
        ss << "},";
        ss << "\"components\" : [";
        ss << "{";
        ss << "\"typeId\": \"PLATFORM_BUILDER\",";
        ss << "\"serializedData\" : {";
        ss << "\"size\": 1,";
        ss << "\"type\" : 0,";
        ss << "\"bouncy\": " << (isBouncy ? "true" : "false") << "";
        ss << "}";
        ss << "},";
        ss << "{\"typeId\": \"COLLISION_SOUND\",\"serializedData\": { \"sound_file\": "
            << (isBouncy ? "\"data/iceplatformcrack.mp3\", \"sound_volume\": 55}"
                : "\"data/350903__cabled_mess__jump_c_03.wav\", \"sound_volume\": 85}") << "}";
        ss << "]";
        ss << "}";
        platformSpawnParameters.Parse(ss.str().c_str());
        platformPtr->Init(platformSpawnParameters);
    }
}