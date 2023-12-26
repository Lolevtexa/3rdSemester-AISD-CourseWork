#pragma once
#include "Libraries.h"
#include "Constants.h"
#include "Button.h"
#include "Toggle.h"
#include "NumberSelection.h"

class ToolBar : 
    public sf::Drawable {
private:
    static const sf::Color background;
    static const sf::Color outline;

    sf::RectangleShape toolBar;

    std::vector<Button> buttons;
    std::vector<Toggle> toggles;
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
        int y = getToolBarHeight();
        buttons.push_back(Button(filename, Constants::TOOL_BAR_ICON_OUTLINE, y, func));
    }

    template<typename Func>
    void addToggle(const std::string& filename, Func func) {
        int y = getToolBarHeight();
        toggles.push_back(Toggle(filename, Constants::TOOL_BAR_ICON_OUTLINE, y, func));
    }   

    void addNumberSelection() {
        int y = getToolBarHeight();
        numberSelections.push_back(NumberSelection(Constants::TOOL_BAR_ICON_OUTLINE, y));
    }

    int getSandNumber(int index = 0) {
        return numberSelections[index].getNumber();
    }

    void eventProcessing(sf::Event event) {
        for (Button& button : buttons) {
            button.eventProcessing(event);
        }

        for (Toggle& toggle : toggles) {
            toggle.eventProcessing(event);
        }

        for (NumberSelection& numberSelection : numberSelections) {
            numberSelection.eventProcessing(event);
        }
    }

    void update() {
        for (Button& button : buttons) {
            button.update();
        }

        for (Toggle& toggle : toggles) {
            toggle.update();
        }

        for (NumberSelection& numberSelection : numberSelections) {
            numberSelection.update();
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(toolBar, states);

        for (const Button& button : buttons) {
            target.draw(button, states);
        }

        for (const Toggle& toggle : toggles) {
            target.draw(toggle, states);
        }

        for (const NumberSelection& numberSelection : numberSelections) {
            target.draw(numberSelection, states);
        }
    }

private:
    int getButtonsHeight() {
        return buttons.size() * Constants::TOOL_BAR_WIDTH;
    }

    int getTogglesHeight() {
        return toggles.size() * Constants::TOOL_BAR_WIDTH;
    }

    int getNumberSelectionsHeight() {
        return numberSelections.size() * Constants::TOOL_BAR_WIDTH;
    }

    int getToolBarHeight() {
        return Constants::TOOL_BAR_ICON_OUTLINE + getButtonsHeight() + getTogglesHeight() + getNumberSelectionsHeight();
    }
};

const sf::Color ToolBar::background = sf::Color(200, 200, 200);
const sf::Color ToolBar::outline = sf::Color(0, 0, 0);
