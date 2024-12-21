#include "ComponentController.h"

#include "glm/glm.hpp"
#include "SDL.h"

#include "Engine/MyEngine.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "ComponentPlatformBounce.h"

void ComponentController::Init(rapidjson::Value& serializedData) {
	auto gameObject = GetGameObject().lock();
	if (!gameObject)
		return;

	_body = gameObject->FindComponent<ComponentPhysicsBody>();
}

void ComponentController::Update(float deltaTime) {
	auto body = _body.lock();
	if (!body)
		return;

	auto linearVelocity = body->getLinearVelocity();
	linearVelocity.x = _mov.x * _movSpeed;
	body->setLinearVelocity(linearVelocity);

	if (_jump) {
		body->addImpulse(glm::vec2(0, _jumpStrength));
		_jump = false;
	}
}

void ComponentController::KeyEvent(SDL_Event& event) {


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
	auto engine = MyEngine::Engine::GetInstance();
	auto collidedBody = other->GetGameObject().lock();
	auto collidedGameObject = collidedBody.get();
	auto bouncyGround = collidedBody->FindComponent<ComponentPlatformBounce>().lock();
	if (bouncyGround) {
		if (!collidedBody) {
			return;
		}
		engine->RegisterForDestruction(collidedGameObject);
		_jump = true;
	}
	
	else if (manifold->localNormal.y > .99)
		_grounded = true;
}

void ComponentController::OnCollisionEnd(ComponentPhysicsBody* other, b2Manifold* manifold) {
	if (other == nullptr || manifold == nullptr) {
		return;
	}
	if (manifold->localNormal.y > .99)
		_grounded = false;

	
}