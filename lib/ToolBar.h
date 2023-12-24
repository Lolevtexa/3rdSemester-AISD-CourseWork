#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "Button.h"

class ToolBar : public sf::Drawable {
private:
    static const sf::Color background;
    static const sf::Color outline;

    sf::RectangleShape toolBar;

    std::vector<Button> buttons;

public:
    ToolBar() {
        toolBar.setSize(sf::Vector2f(Constants::TOOL_BAR_WIDTH, Constants::HEIGHT));
        toolBar.setPosition(0, 0);
        toolBar.setFillColor(background);
        toolBar.setOutlineColor(outline);
        toolBar.setOutlineThickness(-0.5f);
    }

    template<typename Func>
    void addButton(const std::string& filename, Func func) {
        int y = Constants::TOOL_BAR_ICON_OUTLINE + buttons.size() * Constants::TOOL_BAR_WIDTH;
        buttons.push_back(Button(filename, Constants::TOOL_BAR_ICON_OUTLINE, y, func));
    }

    void eventProcessing(sf::Event event) {
        for (Button& button : buttons) {
            button.eventProcessing(event);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(toolBar, states);

        for (const Button& button : buttons) {
            target.draw(button, states);
        }
    }
};

const sf::Color ToolBar::background = sf::Color(200, 200, 200);
const sf::Color ToolBar::outline = sf::Color(0, 0, 0);
