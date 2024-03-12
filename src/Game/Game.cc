#include "Game.h"

Game::Game(const std::uint16_t windowWidth, const std::uint16_t windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
    m_menuState = MenuState::Main;
    m_maxZoom = 3U;
    m_zoom = 0U;
    m_playing = false;

    m_gameWidth = 8800U;
    m_gameHeight = 4800U;
    m_tileSize = 32U;
    m_maxTiles = ((m_gameWidth * m_gameHeight) / m_tileSize) / m_tileSize;
}

MenuState Game::GetMenuState() const
{
    return this->m_menuState;
}

void Game::SetPlaying(const bool playing)
{
    this->m_playing = playing;
}

bool Game::GetPlaying() const
{
    return this->m_playing;
}

void Game::SetMenuState(const MenuState menuState)
{
    this->m_menuState = menuState;
}

std::uint8_t Game::GetMaxZoom() const
{
    return this->m_maxZoom;
}

std::uint8_t Game::GetZoom() const
{
    return this->m_zoom;
}

void Game::SetZoom(const std::uint8_t zoom)
{
    this->m_zoom += zoom;
}

std::uint16_t Game::GetWindowWidth() const
{
    return this->m_windowWidth;
}

std::uint16_t Game::GetWindowHeight() const
{
    return this->m_windowHeight;
}

std::uint32_t Game::GetMaxTiles() const
{
    return this->m_maxTiles;
}

std::uint8_t Game::GetTileSize() const
{
    return this->m_tileSize;
}

std::uint16_t Game::GetGameWidth() const
{
    return this->m_gameWidth;
}
std::uint16_t Game::GetGameHeight() const
{
    return this->m_gameHeight;
}

void Game::SetWindowHeight(const std::uint16_t height)
{
    this->m_windowHeight = height;
}
void Game::SetWindowWidth(const std::uint16_t width)
{
    this->m_windowWidth = width;
}

void InitView(Game &game, sf::View &view)
{
    auto center = view.getCenter();

    view.zoom(0.5F);
    game.SetZoom(1U);

    view.move({-(center.x / 2), -(center.y / 2)});
}

void UpdateView(const Game &game, sf::View &view)
{
    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();
    auto size = view.getSize();
    auto center = view.getCenter();

    if (center.x >= width - (size.x / 2U))
    {
        view.setCenter({width - (size.x / 2U), center.y});
        center = view.getCenter();
    }
    if (center.x <= 0U + (size.x / 2U))
    {
        view.setCenter({size.x / 2U, center.y});
        center = view.getCenter();
    }
    if (center.y <= 0U + (size.y / 2U))
    {
        view.setCenter({center.x, size.y / 2U});
        center = view.getCenter();
    }
    if (center.y >= height - (size.y / 2U))
    {
        view.setCenter({center.x, height - (size.y / 2U)});
        center = view.getCenter();
    }
}

void HandleViewPosition(const sf::RenderWindow &window, const Game &game, sf::View &view)
{
    // get the current mouse position in the window
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    // convert it to world coordinates
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

    auto width = game.GetGameWidth();
    auto height = game.GetGameHeight();

    auto center = view.getCenter();
    auto size = view.getSize();

    auto moveX = worldPos.x - center.x;
    auto moveY = worldPos.y - center.y;

    // RIGHT
    if (moveX > 0U)
    {
        if (moveX + center.x >= width - (size.x / 2U))
        {
            view.setCenter({width - (size.x / 2U), center.y});
        }
        else
        {
            view.move({moveX, 0U});
        }
    }
    else // LEFT
    {
        if (center.x + (moveX) <= 0U + (size.x / 2U))
        {
            view.setCenter({size.x / 2U, center.y});
        }
        else
        {
            view.move({moveX, 0U});
        }
    }
    center = view.getCenter();

    // UP
    if (moveY < 0U)
    {
        if (moveY + center.y <= 0U + (size.y / 2U))
        {
            view.setCenter({center.x, size.y / 2U});
        }
        else
        {
            view.move({0U, moveY});
        }
    }
    else // DOWN
    {
        if (moveY + center.y >= height - (size.y / 2U))
        {
            view.setCenter({center.x, height - (size.y / 2U)});
        }
        else
        {
            view.move({0U, moveY});
        }
    }
}
