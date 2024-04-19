#include "EventHandler.h"

EventHandler::EventHandler()
{
}

bool EventHandler::GetBreak() const
{
    return m_break;
}

void EventHandler::ResetBreak()
{
    m_break = false;
}

// QUIT
void EventHandler::Quit(sf::RenderWindow &window)
{
    window.close();
}

void EventHandler::MouseBtnPressed(sf::RenderWindow &window, Game &game)
{
    auto state = game.GetMenuState();

    if (state != MenuState::Playing)
        BtnPressed(window, game);
    else if (game.GetPlaying() && state == MenuState::Playing)
        game.HandleViewPosition(window);
}

void EventHandler::Playing(Game &game, const sf::Keyboard::Key &key)
{
    auto player = game.GetPlayer();

    switch (key)
    {
    case sf::Keyboard::Key::Escape:
        game.SetMenuState(MenuState::Pause);
        m_break = true;
        break;
    case sf::Keyboard::Key::I:
        game.SetMenuState(MenuState::Inventory);
        m_break = true;
        break;
    case sf::Keyboard::Key::A:
        player->SetMove(PlayerMove::Left);
        break;
    case sf::Keyboard::Key::D:
        player->SetMove(PlayerMove::Right);
        break;
    case sf::Keyboard::Key::W:
        player->SetMove(PlayerMove::Up);
        break;
    case sf::Keyboard::Key::S:
        player->SetMove(PlayerMove::Down);
        break;
    case sf::Keyboard::Key::E:
        player->Interact(game);
        m_break = true;
        break;
    default:
        break;
    }
}

void EventHandler::Inventory(Game &game, const sf::Keyboard::Key &key)
{
    switch (key)
    {
    case sf::Keyboard::Key::I:
        game.SetMenuState(MenuState::Playing);
        m_break = true;
        break;
    default:
        break;
    }
}

void EventHandler::Pause(Game &game, const sf::Keyboard::Key &key)
{
    switch (key)
    {
    case sf::Keyboard::Key::Escape:
        game.SetMenuState(MenuState::Playing);
        m_break = true;
        break;
    default:
        break;
    }
}

void EventHandler::Options(Game &game, const sf::Keyboard::Key &key)
{
    switch (key)
    {
    case sf::Keyboard::Key::Escape:
        game.SetMenuState(MenuState::Pause);
        break;
    default:
        break;
    }
}

void EventHandler::KeyPressed(Game &game, const sf::Keyboard::Key &key)
{
    auto state = game.GetMenuState();

    if (game.GetPlaying())
    {
        switch (state)
        {
        case MenuState::Playing:
            Playing(game, key);
            break;
        case MenuState::Inventory:
            Inventory(game, key);
            break;
        case MenuState::Pause:
            Pause(game, key);
            break;
        case MenuState::Options:
            Options(game, key);
            break;

        default:
            break;
        }
    }
}
void EventHandler::TxtEntered(const Game &game, const sf::Uint32 character)
{
    auto state = game.GetMenuState();

    if (state == MenuState::Create)
    {
        auto width = game.GetWindowWidth();
        for (const auto &data : game.GetInput())
        {
            if (data->GetMenuState() == MenuState::Create)
            {
                if (character == 0x00000008)
                    data->Popback(width);
                else
                    data->Write(width, character);
            }
        }
    }
}
void EventHandler::KeyReleased(const Game &game)
{
    auto state = game.GetMenuState();
    auto player = game.GetPlayer();

    if (game.GetPlaying() && state == MenuState::Playing)
    {
        bool keyPressed =
            (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S));

        if (!keyPressed)
            player->SetMove(PlayerMove::NotMoving);
    }
}
void EventHandler::MouseWheelScrolled(Game &game, float delta)
{
    auto state = game.GetMenuState();

    if (game.GetPlaying() && state == MenuState::Playing)
        game.UpdateZoom(delta);
}

void EventHandler::BtnPressed(sf::RenderWindow &window, Game &game)
{
    // get the current mouse position in the window
    auto pixelPos = sf::Mouse::getPosition(window);
    // convert it to world coordinates
    auto worldPos = window.mapPixelToCoords(pixelPos);

    auto saveFiles = game.GetSaveFiles();
    auto menuState = game.GetMenuState();
    auto btns = game.GetBtn();

    if (menuState == MenuState::OpenLoad)
    {
        for (size_t i = 1; const auto &data : saveFiles)
        {
            data->setColor(sf::Color(255, 255, 255));
            auto txtPos = data->getPosition();
            auto txtLSize = data->getLocalBounds().getSize();

            if (worldPos.x > txtPos.x && worldPos.x < txtPos.x + txtLSize.x && worldPos.y > txtPos.y &&
                worldPos.y < txtPos.y + txtLSize.y)
            {
                game.SetSaveGameID(i);
                data->setColor(sf::Color(0, 255, 0));
            }
            ++i;
        }
    }

    for (const auto &data : btns)
    {
        auto btnPos = data->GetSprite()->getPosition();
        auto btnLSize = data->GetSprite()->getLocalBounds().getSize();
        auto btnScale = data->GetSprite()->getScale();
        auto state = data->GetMenuState();
        bool created = false;

        if (worldPos.x > btnPos.x && worldPos.x < btnPos.x + (btnLSize.x * btnScale.x) && worldPos.y > btnPos.y &&
            worldPos.y < btnPos.y + (btnLSize.y * btnScale.y) && menuState == state)
        {
            switch (data->GetBtnFnc())
            {
            case BtnFunc::Play:
                game.InitPlayer(window);
                m_break = true;
                break;
            case BtnFunc::Resume:
                game.SetMenuState(MenuState::Playing);
                m_break = true;
                break;
            case BtnFunc::Quit:
                Quit(window);
                break;
            case BtnFunc::Options:
                game.SetMenuState(MenuState::Options);
                m_break = true;
                break;
            case BtnFunc::Back:
                if (game.GetPlaying())
                {
                    if (menuState == MenuState::Inventory)
                        game.SetMenuState(MenuState::Playing);
                    else
                        game.SetMenuState(MenuState::Pause);
                }
                else
                    game.SetMenuState(MenuState::Main);

                m_break = true;
                break;
            case BtnFunc::Resolution:
                game.SetWindowHeight(1080U);
                game.SetWindowWidth(1920U);
                game.ResizeWindow(window);
                m_break = true;
                break;
            case BtnFunc::Fullscreen:
                m_break = true;
                break;
            case BtnFunc::Save:
                game.SetMenuState(MenuState::Save);
                game.Saving(false);
                m_break = true;
                break;
            case BtnFunc::OpenLoad:
                game.SetMenuState(MenuState::OpenLoad);
                game.CreateLoadMenu();
                m_break = true;
                break;
            case BtnFunc::Create:
                game.SetMenuState(MenuState::Create);
                game.ResetInputToDefault();
                m_break = true;
                break;
            case BtnFunc::Load:
                if (game.GetSaveGameID() > 0)
                {
                    game.SetMenuState(MenuState::Load);
                    game.InitPlayer(window);
                    m_break = true;
                }
                break;
            case BtnFunc::MainMenu:
                game.Unload();
                m_break = true;
                break;
            default:
                break;
            }
            break;
        }
    }
}
