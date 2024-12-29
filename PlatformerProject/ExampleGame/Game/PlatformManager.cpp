#include "PlatformManager.h"
#include "Engine/MyEngine.h"
#include "Engine/ComponentFactory.h"
#include "rapidjson/document.h"
#include <sstream>

void PlatformManager::Init(rapidjson::Value& serializedData) {
    // Create a parent object for all platforms when the game starts
    auto engine = MyEngine::Engine::GetInstance();
    _platformsParent = engine->CreateGameObject("PlatformsContainer");
}

void PlatformManager::CleanupPlatforms() {
    auto engine = MyEngine::Engine::GetInstance();
    if (auto parent = _platformsParent.lock()) {
        engine->RegisterForDestruction(parent.get());
    }
    // Reset counters and create new parent
    platformCounter = 0;
    _playerMaxHeight = 0;
    _platformsParent = engine->CreateGameObject("PlatformsContainer");
}

void PlatformManager::Update(float deltaTime) {
    auto player = GetGameObject().lock();
    if (!player) return;

    float currentHeight = player->GetPosition().y;
    int platformSpacing = 200;

    if ((int)currentHeight / platformSpacing > (int)_playerMaxHeight / platformSpacing) {
        _playerMaxHeight = currentHeight;

        // Get the platforms parent object
        auto platformsParent = _platformsParent.lock();
        if (!platformsParent) {
            // If parent was lost, recreate it
            auto engine = MyEngine::Engine::GetInstance();
            _platformsParent = engine->CreateGameObject("PlatformsContainer");
            platformsParent = _platformsParent.lock();
        }

        // Create platform as child of platforms parent
        auto platform = MyEngine::Engine::GetInstance()->CreateGameObject(
            "NewPlatform" + std::to_string(platformCounter),
            _platformsParent
        );
        platformCounter++;

        auto platformPtr = platform.lock();
        if (!platformPtr) return;

        // Rest of platform spawn logic
        glm::vec2 screenSize = MyEngine::Engine::GetInstance()->GetScreenSize();
        int minX = -183;  // Match the left boundary where player wraps
        int maxX = 497;   // Match the right boundary where player wraps
        int spawnPositionX = rand() % (maxX - minX + 1) + minX;
        int spawnPositionY = _playerMaxHeight + (int)screenSize.y / 2 + 100;

        bool isBouncy = rand() % 100 < 35;
        bool spawnJetpack = rand() % 100 < 15;
        bool isMoving = rand() % 100 < 10;

        // Spawn jetpack if rolled (as child of platforms parent)
        if (spawnJetpack) {
            auto jetpack = MyEngine::Engine::GetInstance()->CreateGameObject(
                "Jetpack" + std::to_string(platformCounter),
                _platformsParent
            );
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
        ss << "}";
        ss << ",{\"typeId\": \"COLLISION_SOUND\",\"serializedData\": { \"sound_file\": " << (isBouncy ? "\"data/iceplatformcrack.mp3\", \"sound_volume\": 55}" : "\"data/350903__cabled_mess__jump_c_03.wav\", \"sound_volume\": 85}") << "}";
        ss << "" << (isMoving ? ",{\"typeId\": \"PLATFORM_MOVER\", \"serializedData\" : {\"yoyo\": true, \"duration\" : 2.0, \"start\" : [0, 0, 0] , \"end\" : [200, 0, 0] , \"easing\" : 2}}" : "");
        ss << "]";
        ss << "}";
        platformSpawnParameters.Parse(ss.str().c_str());
        platformPtr->Init(platformSpawnParameters);
    }
}