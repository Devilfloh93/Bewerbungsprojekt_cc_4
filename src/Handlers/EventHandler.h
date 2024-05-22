#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

class Game;
class Player;

class EventHandler final
{
public:
    EventHandler();
    ~EventHandler() = default;

    bool GetBreak() const;
    void ResetBreak();

    void MouseBtnPressed(Game &game);
    void KeyPressed(Game &game, const sf::Keyboard::Key &key);
    void TxtEntered(Game &game, const sf::Uint32 character);
    void KeyReleased(const Game &game);
    void MouseWheelScrolled(Game &game, float delta);

    void BtnPressed(Game &game);
    void Playing(Game &game, const sf::Keyboard::Key &key);
    void Inventory(Game &game, const sf::Keyboard::Key &key);
    void Pause(Game &game, const sf::Keyboard::Key &key);
    void Options(Game &game, const sf::Keyboard::Key &key);

    // QUIT
    void Quit(Game &game);

private:
    bool m_break;
};
