#include "ComponentUIButton.h"
#include "Engine/MyEngine.h"

void ComponentUIButton::Init(rapidjson::Value& serializedData) {
    auto gameObject = GetGameObject().lock();
    if (!gameObject) return;

    // Create and store the sprite renderer component
    auto spriteRenderer = gameObject->CreateComponent<ComponentRendererSprite>();
    _spriteRenderer = spriteRenderer;

    if (auto renderer = spriteRenderer.lock()) {
        renderer->SetSprite("bunny-ui", "button_rectangle_depth_line.png");
    }

    _buttonText = serializedData.HasMember("text") ? serializedData["text"].GetString() : "";
}

void ComponentUIButton::Update(float deltaTime) {
    if (!_isVisible) return;

    // Here we could add hover effects or other button states
}

void ComponentUIButton::Render(sre::RenderPass& renderPass) {
    if (!_isVisible) return;

    auto spriteRenderer = _spriteRenderer.lock();
    if (!spriteRenderer) return;

    // Let the sprite renderer handle the actual rendering
    spriteRenderer->Render(renderPass);

    // TODO: Add text rendering on top of the button
    // This would require implementing text rendering in the engine
}