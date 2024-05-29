#pragma once
#include "Text.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Game;
class Player;

enum class Clicked
{
    None = 0,
    Single,
    Double
};

class EventHandler final
{
public:
    EventHandler();
    ~EventHandler() = default;

    bool GetBreak() const;
    void ResetBreak();

    void MousePressed(Game &game);
    void KeyPressed(Game &game, const sf::Keyboard::Key &key);
    void TxtEntered(Game &game, const sf::Uint32 character);
    void KeyReleased(const Game &game);
    void MouseWheelScrolled(Game &game, float delta);
    Clicked CheckTextClicked(Game &game, const vector<unique_ptr<SelectableText>> *selectableTexts);

    void Playing(Game &game, const sf::Keyboard::Key &key);

    // QUIT
    void Quit(Game &game);

private:
    bool m_break;
};
