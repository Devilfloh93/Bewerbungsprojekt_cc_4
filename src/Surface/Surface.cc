#include "Surface.h"
#include <random>

Surface::Surface(const sf::Sprite sprite, const float speed) : m_sprite(sprite), m_speed(speed)
{
}

Grass::Grass(const sf::Sprite sprite, const float speed) : Surface(sprite, speed)
{
}

Water::Water(const sf::Sprite sprite, const float speed) : Surface(sprite, speed)
{
}

sf::Sprite Surface::GetSprite() const
{
    return this->m_sprite;
}

float Surface::GetSpeed() const
{
    return this->m_speed;
}

void InitSurface(std::vector<std::unique_ptr<Surface>> &surfaces,
                 const std::uint32_t tileSize,
                 const std::uint32_t maxTiles,
                 const std::uint32_t width,
                 const std::uint32_t height,
                 const sf::Texture &texture)
{
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()

    // Random Grass Texture Selector
    std::uniform_int_distribution<> dist(0U, 2U);

    size_t j = 0;
    size_t k = 0;
    sf::Sprite tileSprite;

    for (size_t i = 0; i < maxTiles; ++i)
    {
        auto surfaceType = SurfaceType::Grass;
        if (j * tileSize >= width)
        {
            j = 0;
            ++k;
        }

        tileSprite.setTexture(texture);

        if (j == 0 && k == 0 || (j * tileSize == (width - tileSize) && k == 0) ||
            (k * tileSize == (height - tileSize) && j == 0) ||
            (j * tileSize == (width - tileSize) && k * tileSize == (height - tileSize)))
        {
            // FULL WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 416U, tileSize, tileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (j == 0)
        {
            // LEFT WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 320U, tileSize, tileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (j * tileSize == (width - tileSize))
        {
            // RIGHT WATER
            tileSprite.setTextureRect(sf::IntRect(0U, 288U, tileSize, tileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (k == 0)
        {
            // TOP WATER
            tileSprite.setTextureRect(sf::IntRect(183U, 320U, tileSize, tileSize));
            surfaceType = SurfaceType::Water;
        }
        else if (k * tileSize == (height - tileSize))
        {
            // BOT WATER
            tileSprite.setTextureRect(sf::IntRect(128U, 288U, tileSize, tileSize));
            surfaceType = SurfaceType::Water;
        }
        else
        {
            switch (dist(gen))
            {
            case 0:
                tileSprite.setTextureRect(sf::IntRect(0U, 192U, tileSize, tileSize));
                break;
            case 1:
                tileSprite.setTextureRect(sf::IntRect(30U, 192U, tileSize, tileSize));
                break;
            case 2:
                tileSprite.setTextureRect(sf::IntRect(63U, 192U, tileSize, tileSize));
                break;
            case 3:
                tileSprite.setTextureRect(sf::IntRect(96U, 192U, tileSize, tileSize));
                break;

            default:
                break;
            }
            surfaceType = SurfaceType::Grass;
        }

        tileSprite.setPosition(j * tileSize, k * tileSize);

        switch (surfaceType)
        {
        case SurfaceType::Grass:
            surfaces.push_back(std::make_unique<Grass>(tileSprite, 1.5F));
            break;
        case SurfaceType::Water:
            surfaces.push_back(std::make_unique<Water>(tileSprite, 0.8F));
            break;

        default:
            break;
        }

        ++j;
    }
}
