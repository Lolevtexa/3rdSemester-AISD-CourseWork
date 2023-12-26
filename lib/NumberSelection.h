#pragma once
#include "Libraries.h"
#include "Constants.h"

class NumberSelection : 
    public sf::Drawable {
private:
    static const sf::Color background;
    static const sf::Color outline;

    sf::RectangleShape numberSelection;
    sf::Text numberText;

    int number;
    int minNumber;
    int maxNumber;

public:
    NumberSelection(int x, int y, int defaultNumber = 1, int minNumber = 1, int maxNumber = 1000) : 
        number(defaultNumber), 
        minNumber(minNumber), 
        maxNumber(maxNumber) {
        numberSelection.setSize(sf::Vector2f(Constants::NUMBER_SELECTION_WIDTH, Constants::NUMBER_SELECTION_HEIGHT));
        numberSelection.setPosition(x, y);
        numberSelection.setFillColor(background);
        numberSelection.setOutlineColor(outline);
        numberSelection.setOutlineThickness(-1.f);

        numberText.setFont(Constants::NUMBER_SELECTION_FONT);
        numberText.setString(std::to_string(number));
        numberText.setCharacterSize(Constants::NUMBER_SELECTION_TEXT_SIZE);
        numberText.setFillColor(Constants::NUMBER_SELECTION_TEXT_COLOR);
        numberText.setPosition(getCenter());
    }

    void eventProcessing(sf::Event event) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (numberSelection.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    number = spiner(number + 1);
                    numberText.setString(std::to_string(number));
                    int x = numberSelection.getPosition().x;
                    int y = numberSelection.getPosition().y;
                    numberText.setPosition(getCenter());
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                if (numberSelection.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    number = spiner(number - 1);
                    numberText.setString(std::to_string(number));
                    numberText.setPosition(getCenter());
                }
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(numberSelection, states);
        target.draw(numberText, states);
    }

    int getNumber() {
        return number;
    }

private:
    sf::Vector2f getCenter() {
        float x = numberSelection.getPosition().x + Constants::NUMBER_SELECTION_WIDTH / 2 - numberText.getGlobalBounds().width / 2;
        float y = numberSelection.getPosition().y + Constants::NUMBER_SELECTION_HEIGHT / 2 - numberText.getCharacterSize() / 2;
        return sf::Vector2f(x, y);
    }

    int spiner(int number) {
        if (number < minNumber) {
            return maxNumber;
        }
        if (number > maxNumber) {
            return minNumber;
        }
        return number;
    }
};

const sf::Color NumberSelection::background = sf::Color(200, 200, 200);
const sf::Color NumberSelection::outline = sf::Color(0, 0, 0);
