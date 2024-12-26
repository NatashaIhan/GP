#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentRendererSprite.h"

// This component handles UI buttons in the game using sprites
class ComponentUIButton : public MyEngine::Component {
public:
    // Override the necessary component virtual functions
    virtual void Init(rapidjson::Value& serializedData) override;
    virtual void Render(sre::RenderPass& renderPass) override;
    virtual void Update(float deltaTime) override;

    // Method to control button visibility
    // When visible=false, the button won't be rendered
    void SetVisible(bool visible) { _isVisible = visible; }

    // Method to get current visibility state
    bool IsVisible() const { return _isVisible; }

    // Method to set the button's text (for future text rendering)
    void SetText(const std::string& text) { _buttonText = text; }

    // Method to get the current button text
    const std::string& GetText() const { return _buttonText; }

private:
    // The sprite renderer that will display our button image
    std::weak_ptr<ComponentRendererSprite> _spriteRenderer;

    // Whether the button should be visible
    bool _isVisible = false;

    // The text to display on the button (for future text rendering)
    std::string _buttonText;
};