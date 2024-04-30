#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

class Game;
class Player;

class EventHandler
{
public:
    EventHandler();
    ~EventHandler() = default;

    bool GetBreak() const;
    void ResetBreak();

    void MouseBtnPressed(sf::RenderWindow &window, Game &game);
    void KeyPressed(Game &game, const sf::Keyboard::Key &key);
    void TxtEntered(const Game &game, const sf::Uint32 character);
    void KeyReleased(const Game &game);
    void MouseWheelScrolled(Game &game, float delta);

    void BtnPressed(sf::RenderWindow &window, Game &game);
    void Playing(Game &game, const sf::Keyboard::Key &key);
    void Inventory(Game &game, const sf::Keyboard::Key &key);
    void Pause(Game &game, const sf::Keyboard::Key &key);
    void Options(Game &game, const sf::Keyboard::Key &key);

    // QUIT
    void Quit(sf::RenderWindow &window);

private:
    bool m_break;
};
