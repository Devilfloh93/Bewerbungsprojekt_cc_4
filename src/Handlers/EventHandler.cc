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

void EventHandler::MousePressed(Game &game)
{
    auto state = game.GetMenuState().first;

    if (state != MenuState::Playing)
    {
        Utilities utilities;
        auto window = game.GetWindow();
        // get the current mouse position in the window
        auto pixelPos = sf::Mouse::getPosition(*window);
        // convert it to world coordinates
        auto worldPos = window->mapPixelToCoords(pixelPos);

        auto menuState = game.GetMenuState().first;
        auto btns = game.GetBtn();

        if (menuState == MenuState::OpenLoad || menuState == MenuState::Trader || menuState == MenuState::Inventory)
        {
            auto dialogText = game.GetDialogText();
            CheckTextClicked(game, dialogText);
        }

        if (menuState == MenuState::Hotkeys)
        {
            auto hotkeyMenu = game.GetHotkeyMenu();
            auto clicked = CheckTextClicked(game, hotkeyMenu);

            if (clicked == Clicked::Double)
                game.SetChangeHotkeyAllowed(true);
            else
                game.SetChangeHotkeyAllowed(false);
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
                auto player = game.GetPlayer();
                switch (data->GetBtnFnc())
                {
                case BtnFunc::Play:
                    game.InitPlayer();
                    break;
                case BtnFunc::Resume:
                    game.SetMenuState(MenuState::Playing, false);
                    break;
                case BtnFunc::Quit:
                    Quit(game);
                    break;
                case BtnFunc::Options:
                    game.SetMenuState(MenuState::Options, false);
                    break;
                case BtnFunc::Back:
                    game.SetMenuState();
                    break;
                case BtnFunc::OpenResolution:
                    game.SetMenuState(MenuState::Resolution, false);
                    break;
                case BtnFunc::Resolution1280x720:
                    game.ResizeWindow(1280U, 720U);
                    break;
                case BtnFunc::Resolution1920x1080:
                    game.ResizeWindow(1920U, 1080U);
                    break;
                case BtnFunc::Fullscreen:
                    game.FullscreenWindow();
                    break;
                case BtnFunc::Save:
                    game.SetMenuState(MenuState::Save, false);
                    game.Saving(false);
                    break;
                case BtnFunc::OpenLoad:
                    game.SetMenuState(MenuState::OpenLoad, true);
                    game.CreateLoadMenu();
                    break;
                case BtnFunc::Create:
                    game.SetMenuState(MenuState::Create, false);
                    game.SetSelectedInput(nullptr);
                    game.ResetInputToDefault();
                    break;
                case BtnFunc::Load:
                    if (game.GetDialogSelectedID(SelectedTextCategorie::Nothing) != 0)
                    {
                        game.SetMenuState(MenuState::Load, false);
                        game.InitPlayer();
                    }
                    else
                        game.AddMessage(11, MessageType::Error);
                    break;
                case BtnFunc::MainMenu:
                    game.Unload();
                    break;
                case BtnFunc::Language:
                    game.SetMenuState(MenuState::Language, false);
                    break;
                case BtnFunc::ChangeLanguageEN:
                    game.ChangeLanguage("enEN");
                    break;
                case BtnFunc::ChangeLanguageDE:
                    game.ChangeLanguage("deDE");
                    break;
                case BtnFunc::OpenHotkeys:
                    game.SetMenuState(MenuState::Hotkeys, true);
                    break;
                case BtnFunc::Buy:
                    player->GetTrader()->Buy(game);
                    break;
                case BtnFunc::Sell:
                    player->GetTrader()->Sell(game);
                    break;
                case BtnFunc::Use:
                    player->UseItem(game);
                    break;
                case BtnFunc::ResetHotkeys:
                    game.ResetHotkeys();
                    break;
                default:
                    break;
                }
                break;
            }
        }

        m_break = true;
    }
    else if (game.GetPlaying() && state == MenuState::Playing)
    {
        auto window = game.GetWindow();
        // get the current mouse position in the window
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        // convert it to world coordinates
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

        game.UpdateViewPosition(worldPos.x, worldPos.y);
    }
}

void EventHandler::Playing(Game &game, const sf::Keyboard::Key &key)
{
    Collision collision;
    Utilities utilities;
    auto player = game.GetPlayer();
    auto playerPos = player->GetSprite()->getPosition();

    auto hotkey = utilities.GetHotkey(game, key);

    switch (hotkey)
    {
    case Hotkey::Interact:
        player->Interact(game);
        m_break = true;
        break;
    case Hotkey::Escape:
        game.SetMenuState(MenuState::Pause, false);
        m_break = true;
        break;
    case Hotkey::Inventory:
        game.SetMenuState(MenuState::Inventory, true);
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
}

void EventHandler::KeyPressed(Game &game, const sf::Keyboard::Key &key)
{
    Utilities utilities;
    auto state = game.GetMenuState().first;
    auto changeHotkey = game.GetChangeHotkeyData().allowed;

    if (key >= 0 && key <= 25 && changeHotkey)
        game.SetNewHotkey(key);

    if (game.GetPlaying() && state == MenuState::Playing)
        Playing(game, key);
    else
    {
        auto hotkey = utilities.GetHotkey(game, key);

        if (hotkey == Hotkey::Escape)
        {
            game.SetMenuState();
            m_break = true;
        }
    }
}

void EventHandler::TxtEntered(Game &game, const sf::Uint32 character)
{
    auto selectedInput = game.GetSelectedInput();
    if (selectedInput != nullptr)
    {
        auto state = game.GetMenuState().first;
        auto windowSize = game.GetWindowSize();

        for (const auto &data : game.GetInput())
        {
            if (data->GetMenuState() == state && selectedInput == data)
            {
                if (character == 0x00000008)
                    data->Popback(windowSize.width);
                else
                    data->Write(windowSize.width, character);
            }
        }
    }
}

void EventHandler::KeyReleased(const Game &game)
{
    auto state = game.GetMenuState().first;
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
    auto state = game.GetMenuState().first;

    if (game.GetPlaying() && state == MenuState::Playing)
        game.UpdateZoom(delta);
}

Clicked EventHandler::CheckTextClicked(Game &game, const vector<unique_ptr<SelectableText>> *selectableTexts)
{
    Utilities utilities;
    Clicked clicked = Clicked::None;
    auto window = game.GetWindow();
    // get the current mouse position in the window
    auto pixelPos = sf::Mouse::getPosition(*window);
    // convert it to world coordinates
    auto worldPos = window->mapPixelToCoords(pixelPos);

    for (const auto &data : *selectableTexts)
    {
        auto selectableText = data.get();

        auto text = selectableText->GetText();
        auto newSelectedTextID = selectableText->GetSelectedTextID();

        auto txtPos = text->getPosition();
        auto txtLSize = text->getLocalBounds().getSize();

        clicked = utilities.CheckTextClicked(worldPos, txtPos, txtLSize);

        if (clicked == Clicked::Single)
        {
            game.SetSelectedTextID(newSelectedTextID);
            selectableText->SetDoubleClicked(false);
            break;
        }
        else if (clicked == Clicked::Double)
        {
            game.SetSelectedTextID(newSelectedTextID);
            selectableText->SetDoubleClicked(true);
            break;
        }
    }

    return clicked;
}
