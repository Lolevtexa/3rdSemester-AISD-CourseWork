#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "Button.h"

class ToolBar {
private:
    static const sf::Color background;
    static const sf::Color outline;

    std::vector<Button> buttons;

public:
    template<typename Func>
    void addButton(const std::string& filename, Func func) {
        int y = Constants::TOOL_BAR_ICON_OUTLINE + buttons.size() * Constants::TOOL_BAR_WIDTH;
        Button button(filename, Constants::TOOL_BAR_ICON_OUTLINE, y);
        button.setEventButtonPressed(func);
        buttons.push_back(button);
    }

    void eventProcessing(sf::Event event) {
        for (Button& button : buttons) {
            button.eventProcessing(event);
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape toolBar(sf::Vector2f(Constants::TOOL_BAR_WIDTH, Constants::HEIGHT));
        toolBar.setPosition(0, 0);
        toolBar.setFillColor(background);
        toolBar.setOutlineColor(outline);
        toolBar.setOutlineThickness(-0.5f);
        window.draw(toolBar);

        for (Button& button : buttons) {
            button.draw(window);
        }
    }
};

const sf::Color ToolBar::background = sf::Color(200, 200, 200);
const sf::Color ToolBar::outline = sf::Color(0, 0, 0);
