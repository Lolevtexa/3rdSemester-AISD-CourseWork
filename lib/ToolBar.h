#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "Button.h"
#include "NumberSelection.h"

class ToolBar : public sf::Drawable {
private:
    static const sf::Color background;
    static const sf::Color outline;

    sf::RectangleShape toolBar;

    std::vector<Button> buttons;
    std::vector<NumberSelection> numberSelections;

public:
    ToolBar() {
        toolBar.setSize(sf::Vector2f(Constants::TOOL_BAR_WIDTH, Constants::HEIGHT));
        toolBar.setPosition(0, 0);
        toolBar.setFillColor(background);
        toolBar.setOutlineColor(outline);
        toolBar.setOutlineThickness(-1.f);
    }

    template<typename Func>
    void addButton(const std::string& filename, Func func) {
        int y = Constants::TOOL_BAR_ICON_OUTLINE + (numberSelections.size() + buttons.size()) * Constants::TOOL_BAR_WIDTH;
        buttons.push_back(Button(filename, Constants::TOOL_BAR_ICON_OUTLINE, y, func));
    }

    void addNumberSelection() {
        int y = Constants::TOOL_BAR_ICON_OUTLINE + (numberSelections.size() + buttons.size()) * Constants::TOOL_BAR_WIDTH;
        numberSelections.push_back(NumberSelection(Constants::TOOL_BAR_ICON_OUTLINE, y));
    }

    int getSandNumber(int index = 0) {
        return numberSelections[index].getNumber();
    }

    void eventProcessing(sf::Event event) {
        for (Button& button : buttons) {
            button.eventProcessing(event);
        }

        for (NumberSelection& numberSelection : numberSelections) {
            numberSelection.eventProcessing(event);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(toolBar, states);

        for (const Button& button : buttons) {
            target.draw(button, states);
        }

        for (const NumberSelection& numberSelection : numberSelections) {
            target.draw(numberSelection, states);
        }
    }
};

const sf::Color ToolBar::background = sf::Color(200, 200, 200);
const sf::Color ToolBar::outline = sf::Color(0, 0, 0);
