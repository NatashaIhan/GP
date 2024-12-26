#pragma once

#include "Engine/Component.h"
#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentRendererSprite.h"


class ComponentController : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;
	void OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) override;
	void OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) override;

private:
	std::weak_ptr<ComponentPhysicsBody> _body;
	std::weak_ptr<ComponentRendererSprite> _spriteRenderer;
	glm::vec3 _mov;
	bool _jump;
	bool _grounded;
	bool _jetpack;
	bool _isDead = false;

	float _movSpeed = 2;
	float _jumpStrength = 5;
	float _jetpackStrength = 7;
	float _maxFallDistance = 1100.0f; // Maximum distance player can fall before dying
	float _playerMaxHeight = 0.0f; // Tracks the player's highest point

	// Game time tracking
	float _gameTime = 0.0f;
	float _bestTime = 0.0f;



public:
    bool IsDead() const { return _isDead; }
    float GetGameTime() const { return _gameTime; }
    float GetBestTime() const { return _bestTime; }
    void Reset();
};