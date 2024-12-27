#include "ComponentController.h"

#include "glm/glm.hpp"
#include "SDL.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "Engine/Components/ComponentSoundSystem.h"
#include "ComponentPlatform.h"

#include "ComponentPlatformBounce.h"
#include "ComponentJetpack.h"
#include "ComponentSound.h"
#include "Engine/Components/ComponentRendererSprite.h"




void ComponentController::Init(rapidjson::Value& serializedData) {
	auto gameObject = GetGameObject().lock();
	if (!gameObject)
		return;

	_body = gameObject->FindComponent<ComponentPhysicsBody>();
	// Store reference to the sprite renderer
	_spriteRenderer = gameObject->FindComponent<ComponentRendererSprite>();
}

void ComponentController::Update(float deltaTime) {
	auto body = _body.lock();
	if (!body)
		return;

	// Don't update if dead
	if (_isDead) {
		// Ensure the physics body stays in sensor mode while dead
		auto physicsBody = GetGameObject().lock()->FindComponent<ComponentPhysicsBody>().lock();
		if (physicsBody) {
			physicsBody->_fixture->SetSensor(true);
		}
		return;
	}
	_gameTime += deltaTime;

	auto linearVelocity = body->getLinearVelocity();
	linearVelocity.x = _mov.x * _movSpeed;
	body->setLinearVelocity(linearVelocity);

	if (_jump) {
		body->addImpulse(glm::vec2(0, _jumpStrength));
		_jump = false;
	}

	if (_jetpack) {
		body->addImpulse(glm::vec2(0, _jetpackStrength));
		_jetpack = false;
	}

	//using hardcoded values, should probably fit to screen size given time.
	auto playerObject = body->GetGameObject().lock();
	glm::vec3 currentPos = playerObject->GetPosition();

	// First check: Are we at a new highest point?
	if (currentPos[1] > _playerMaxHeight) {
		// If current Y position is higher than our recorded maximum,
		// update _playerMaxHeight to track this new highest point
		_playerMaxHeight = currentPos[1];
	}

	// Second check: Have we fallen too far from our highest point?
	if ((_playerMaxHeight - currentPos[1]) > _maxFallDistance) {
		// Calculate the fall distance by subtracting current Y position
		// from the highest Y position we recorded
		// If this difference is greater than _maxFallDistance (500 units),
		// then the player has fallen too far and should die

		// Set the death flag
		_isDead = true;


		// Make the player's body a sensor so it falls through platforms
		auto physicsBody = GetGameObject().lock()->FindComponent<ComponentPhysicsBody>().lock();
		if (physicsBody) {
			physicsBody->_fixture->SetSensor(true);
		}
		auto currentVel = body->getLinearVelocity();
		body->setLinearVelocity(glm::vec2(0, currentVel.y));

		// Change the sprite to the hurt animation
		auto spriteRenderer = _spriteRenderer.lock();
		if (spriteRenderer) {
			spriteRenderer->SetSprite("bunny-art", "bunny1_hurt.png");
		}
		// Display game over information to the player
		std::cout << "\nGame Over! Fell too far!" << std::endl;
		std::cout << "Fell " << (_playerMaxHeight - currentPos[1]) << " units" << std::endl;
		std::cout << "Time: " << _gameTime << " seconds" << std::endl;
		std::cout << "Best Time: " << _bestTime << " seconds" << std::endl;
		std::cout << "Press R to restart" << std::endl;
		return;
	}

	if (currentPos[0] < -186) {
		glm::vec3 newPos = glm::vec3(497, currentPos[1], currentPos[2]);
		playerObject->SetPosition(newPos);
		body->setPosition(newPos);
	}
	if (currentPos[0] > 500) {
		glm::vec3 newPos = glm::vec3(-183, currentPos[1], currentPos[2]);
		playerObject->SetPosition(newPos);
		body->setPosition(newPos);
	}
}

void ComponentController::KeyEvent(SDL_Event& event) {
	// First check: Is the player dead and pressing R?
	if (_isDead && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
		Reset();  // Call the reset function we created
		return;   // Exit the function after handling restart
	}

	// movement
	switch (event.key.keysym.sym) {
		case SDLK_a: _mov.x = -(event.type == SDL_KEYDOWN); break;
		case SDLK_d: _mov.x = +(event.type == SDL_KEYDOWN); break;
	}

	// jump
	if (_grounded && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
		_jump = true;

}

void ComponentController::OnCollisionStart(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other == nullptr) {
		return;
	}
	//Get engine for handling platform destruction
	auto engine = MyEngine::Engine::GetInstance();

	//Get collision information
	auto collidedBody = other->GetGameObject().lock();
	auto collidedGameObject = collidedBody.get();
	auto soundCollision = collidedBody->FindComponent<ComponentSound>().lock();
	auto platformCollision = collidedBody->FindComponent<ComponentPlatform>().lock();
	auto jetpackCollision = collidedBody->FindComponent<ComponentJetpack>().lock();

	//Get player information and access to system components for playing SFX
	auto playerObject = GetGameObject().lock();
	if (soundCollision) {
		if (!collidedBody) {
			return;
		}
		auto soundSystem = playerObject->FindComponent<ComponentSoundSystem>().lock();
		soundSystem->PlaySFX(soundCollision->GetCollisionSoundFile());
	}

	//Handle jumping and schedule platform destruction.
	if (platformCollision) {
		if (!collidedBody) {
			return;
		}
		_jump = true;
		if (platformCollision->_bouncy) {
			engine->RegisterForDestruction(collidedGameObject);
			return;
		}
	  }
	if (jetpackCollision) {
		if (!collidedBody) {
			return;
		}
		engine->RegisterForDestruction(collidedGameObject);
		_jetpack = true;
	}
}

void ComponentController::OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other == nullptr || manifold == nullptr) {
		return;
	}
	if (manifold->localNormal.y > .99)
		_grounded = false;
	
}

void ComponentController::Reset() {
	// First get the physics body component
	auto body = _body.lock();
	if (!body) return;

	// Get the game object this controller is attached to
	auto gameObject = GetGameObject().lock();
	if (!gameObject) return;

	// Reset position to starting point
	glm::vec3 startPos(100, 300, 0);
	gameObject->SetPosition(startPos);
	body->setPosition(startPos);
	body->setLinearVelocity(glm::vec2(0, 0));

	// Reset physics body's sensor state to enable collisions again
	auto physicsBody = gameObject->FindComponent<ComponentPhysicsBody>().lock();
	if (physicsBody) {
		physicsBody->_fixture->SetSensor(false);
	}

	// Reset sprite back to ready state
	auto spriteRenderer = _spriteRenderer.lock();
	if (spriteRenderer) {
		spriteRenderer->SetSprite("bunny-art", "bunny1_ready.png");
	}

	// Reset game state variables
	_isDead = false;
	_playerMaxHeight = 0.0f;  // Reset the maximum height tracking
	if (_gameTime > _bestTime) {
		_bestTime = _gameTime;
	}
	_gameTime = 0.0f;
	_mov = glm::vec3(0);
	_jump = false;
	_jetpack = false;
}