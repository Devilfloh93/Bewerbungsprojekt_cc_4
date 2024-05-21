#include "EventHandler.h"
#include "Collision.h"
#include "Gui.h"

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
void EventHandler::Quit(Game &game)
{
    game.GetWindow()->close();
}

void EventHandler::MouseBtnPressed(Game &game)
{
    auto state = game.GetMenuState();

    if (state != MenuState::Playing)
        BtnPressed(game);
    else if (game.GetPlaying() && state == MenuState::Playing)
        game.HandleViewPosition();
}

void EventHandler::Playing(Game &game, const sf::Keyboard::Key &key)
{
    Collision collision;
    auto player = game.GetPlayer();
    auto hotkeys = game.GetHotkeys();
    auto playerPos = player->GetSprite()->getPosition();

    for (const auto &data : hotkeys)
    {
        auto hotkey = static_cast<Hotkey>(data.first);
        auto keyID = data.second;

        if (key == keyID)
        {
            switch (hotkey)
            {
            case Hotkey::Interact:
                player->Interact(game);
                m_break = true;
                break;
            case Hotkey::Escape:
                game.SetMenuState(MenuState::Pause);
                m_break = true;
                break;
            case Hotkey::Inventory:
                game.SetMenuState(MenuState::Inventory);
                player->InitInventoryItems(game);
                m_break = true;
                break;
            case Hotkey::LeftMove:
                if (collision.InViewRange(&game, {playerPos.x, playerPos.y}))
                    player->SetMove(Move::Left);
                break;
            case Hotkey::RightMove:
                if (collision.InViewRange(&game, {playerPos.x, playerPos.y}))
                    player->SetMove(Move::Right);

                break;
            case Hotkey::UpMove:
                if (collision.InViewRange(&game, {playerPos.x, playerPos.y}))
                    player->SetMove(Move::Up);
                break;
            case Hotkey::DownMove:
                if (collision.InViewRange(&game, {playerPos.x, playerPos.y}))
                    player->SetMove(Move::Down);
                break;

            default:
                break;
            }
            break;
        }
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

void EventHandler::TxtEntered(Game &game, const sf::Uint32 character)
{
    auto selectedInput = game.GetSelectedInput();
    if (selectedInput != nullptr)
    {
        auto state = game.GetMenuState();
        auto width = game.GetWindowWidth();

        for (const auto &data : game.GetInput())
        {
            if (data->GetMenuState() == state && selectedInput == data)
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
            player->SetMove(Move::NotMoving);
    }
}
void EventHandler::MouseWheelScrolled(Game &game, float delta)
{
    auto state = game.GetMenuState();

    if (game.GetPlaying() && state == MenuState::Playing)
        game.UpdateZoom(delta);
}

void EventHandler::BtnPressed(Game &game)
{
    Utilities utilities;
    auto window = game.GetWindow();
    // get the current mouse position in the window
    auto pixelPos = sf::Mouse::getPosition(*window);
    // convert it to world coordinates
    auto worldPos = window->mapPixelToCoords(pixelPos);

    auto menuState = game.GetMenuState();
    auto btns = game.GetBtn();

    if (menuState == MenuState::OpenLoad)
    {
        auto saveFiles = game.GetSaveFiles();

        for (const auto &data : *saveFiles)
        {
            auto text = data.get()->GetText();
            auto newSelectedTextID = data.get()->GetID();

            auto txtPos = text->getPosition();
            auto txtLSize = text->getLocalBounds().getSize();

            auto clicked = utilities.CheckTextClicked(worldPos, txtPos, txtLSize);
            if (clicked)
            {
                game.SetSelectedTextID(newSelectedTextID);
            }
        }
    }

    if (menuState == MenuState::Trader)
    {
        auto dialogText = game.GetDialogText();
        for (const auto &data : *dialogText)
        {
            auto text = data.get()->GetText();
            auto newSelectedTextID = data.get()->GetID();

            auto txtPos = text->getPosition();
            auto txtLSize = text->getLocalBounds().getSize();
            auto clicked = utilities.CheckTextClicked(worldPos, txtPos, txtLSize);

            if (clicked)
            {
                game.SetSelectedTextID(newSelectedTextID);
            }
        }
    }

    for (const auto &data : game.GetInput())
    {
        auto sprite = data->GetSprite();
        auto btnPos = sprite->getPosition();
        auto btnLSize = sprite->getLocalBounds().getSize();
        auto btnScale = sprite->getScale();

        auto clicked = utilities.CheckSpriteClicked(worldPos, btnPos, btnLSize, btnScale);

        if (data->GetMenuState() == menuState && clicked)
        {
            game.SetSelectedInput(data);
        }
    }

    for (const auto &data : btns)
    {
        auto btnPos = data->GetSprite()->getPosition();
        auto btnLSize = data->GetSprite()->getLocalBounds().getSize();
        auto btnScale = data->GetSprite()->getScale();
        auto state = data->GetMenuState();

        /**
         * @brief Check if Mouse is inside the size of the Btn Size
         *
         */
        auto clicked = utilities.CheckSpriteClicked(worldPos, btnPos, btnLSize, btnScale);
        if (clicked && menuState == state)
        {
            switch (data->GetBtnFnc())
            {
            case BtnFunc::Play:
                game.InitPlayer();
                m_break = true;
                break;
            case BtnFunc::Resume:
                game.SetMenuState(MenuState::Playing);
                m_break = true;
                break;
            case BtnFunc::Quit:
                Quit(game);
                break;
            case BtnFunc::Options:
                game.SetMenuState(MenuState::Options);
                m_break = true;
                break;
            case BtnFunc::Back:
                if (game.GetPlaying())
                {
                    if (menuState == MenuState::Inventory || menuState == MenuState::Trader)
                    {
                        game.SetMenuState(MenuState::Playing);
                    }
                    else
                        game.SetMenuState(MenuState::Pause);
                }
                else
                    game.SetMenuState(MenuState::Main);

                m_break = true;
                break;
            case BtnFunc::OpenResolution:
                game.SetMenuState(MenuState::Resolution);
                m_break = true;
                break;
            case BtnFunc::Resolution1280x720:
                game.ResizeWindow(1280U, 720U);
                m_break = true;
                break;
            case BtnFunc::Resolution1920x1080:
                game.ResizeWindow(1920U, 1080U);
                m_break = true;
                break;
            case BtnFunc::Fullscreen:
                game.FullscreenWindow();
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
                game.SetSelectedInput(nullptr);
                game.ResetInputToDefault();
                m_break = true;
                break;
            case BtnFunc::Load:
                if (game.GetDialogSelectedID(game.GetSaveFiles(), SelectedTextCategorie::Nothing) > 0)
                {
                    game.SetMenuState(MenuState::Load);
                    game.InitPlayer();
                    m_break = true;
                }
                else
                {
                    auto messageFormat = utilities.GetMessageFormat(game, 11);
                    game.AddMessage(messageFormat, MessageType::Error);
                }
                break;
            case BtnFunc::MainMenu:
                game.Unload();
                m_break = true;
                break;
            case BtnFunc::Language:
                game.SetMenuState(MenuState::Language);
                m_break = true;
                break;
            case BtnFunc::ChangeLanguageEN:
                game.ChangeLanguage("enEN");
                m_break = true;
                break;
            case BtnFunc::ChangeLanguageDE:
                game.ChangeLanguage("deDE");
                m_break = true;
                break;
            case BtnFunc::OpenHotkeys:
                game.SetMenuState(MenuState::Hotkeys);
                m_break = true;
                break;
            case BtnFunc::Buy:
                game.GetPlayer()->GetTrader()->Buy(game);
                m_break = true;
                break;
            case BtnFunc::Sell:
                game.GetPlayer()->GetTrader()->Sell(game);
                m_break = true;
                break;
            default:
                break;
            }
            break;
        }
    }
}
