#pragma once
#include "Game.h"
#include <cstdint>

using namespace std;

class Collision
{

public:
    Collision();
    ~Collision() = default;

    /**
     * @brief Check if Sprite is in viewRange of the Camera
     *
     * @return
     */
    bool InViewRange(Game *game, const sf::Vector2f &spritePos);

    /**
     * @brief Checks if the player Right movement gets blocked by a object x coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param playerSize the texture size of the player
     * @param objPos the object pos
     * @param speed the player pixel movement distance
     * @return true if player can move Right or false if not
     */
    bool CanMoveRightX(const float playerPos, const float playerSize, const float objPos, const float speed) const;
    bool CanMoveRightX(const float playerPos, const float objPos, const float speed) const;

    /**
     * @brief Checks if the player Right/Left movement get blocked by a object y coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param playerSize the texture size of the player
     * @param objSize the object size
     * @param objPos the object pos
     * @return true if player can move Right / Left or false if not
     */
    bool CanMoveRightLeftY(const float playerPos,
                           const float playerSize,
                           const float objSize,
                           const float objPos) const;

    /**
    * @brief Checks if the player Right/Left movement get blocked by a object y coordinate texture size - collision size
    *
    * @param playerPos the current position of the player
    * @param objPos the object pos
    * @param objSize the object size
    * @param objCollision the object collision size defined in data/world/worldTemplate.json -> TextureData last 2 entries
    * @return true if player can move Right / Left or false if not
    */
    bool CanMoveRightLeftY(const float playerPos,
                           const float objPos,
                           const float objSize,
                           const uint8_t objCollision) const;

    /**
     * @brief Checks if the player Left movement get blocked by a object x coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param objPos the object pos
     * @param objSize the object size
     * @param speed the player pixel movement distance
     * @return true if player can move Left or false if not
     */
    bool CanMoveLeftX(const float playerPos, const float objPos, const float objSize, const float speed) const;

    /**
     * @brief Checks if the player Left movement get blocked by a object x coordinate texture size - collision size
     *
     * @param playerPos the current position of the player
     * @param objPos the object pos
     * @param objCollision the object collision size defined in data/world/worldTemplate.json -> TextureData last 2 entries
     * @param speed the player pixel movement distance
     * @return true if player can move Left or false if not
     */
    bool CanMoveLeftX(const float playerPos, const float objPos, const uint8_t objCollision, const float speed) const;

    /**
     * @brief Checks if the player Down/Up movement get blocked by a object x coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param playerSize the texture size of the player
     * @param objPos the object pos
     * @param objSize the object size
     * @return true if player can move Down/Up or false if not
     */
    bool CanMoveDownUpX(const float playerPos, const float playerSize, const float objPos, const float objSize) const;
    /**
     * @brief Checks if the player Down/Up movement get blocked by a object x coordinate texture size - collision size
     *
     * @param playerPos the current position of the player
     * @param objPos the object pos
     * @param objCollision the object collision size defined in data/world/worldTemplate.json -> TextureData last 2 entries
     * @return true if player can move Down/Up or false if not
     */
    bool CanMoveDownUpX(const float playerPos, const float objPos, const uint8_t objCollision) const;

    /**
     * @brief Checks if the player Up movement get blocked by a object y coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param playerSize the texture size of the player
     * @param objPos the object pos
     * @param objSize the object size
     * @param speed the player pixel movement distance
     * @return true if player can move Up or false if not
     */
    bool CanMoveUpY(const float playerPos,
                    const float playerSize,
                    const float objPos,
                    const float objSize,
                    const float speed) const;

    /**
     * @brief Checks if the player Up movement get blocked by a object y coordinate texture size - collision size
     *
     * @param playerPos the current position of the player
     * @param objPos the object pos
     * @param objSize the object size
     * @param objCollision the object collision size defined in data/world/worldTemplate.json -> TextureData last 2 entries
     * @param speed the player pixel movement distance
     * @return true if player can move Up or false if not
     */
    bool CanMoveUpY(const float playerPos,
                    const float objPos,
                    const float objSize,
                    const uint8_t objCollision,
                    const float speed) const;

    /**
     * @brief Checks if the player Down movement get blocked by a object y coordinate texture size
     *
     * @param playerPos the current position of the player
     * @param playerSize the texture size of the player
     * @param objPos the object pos
     * @param objSize the object size
     * @param speed the player pixel movement distance
     * @return true if player can move Down or false if not
     */
    bool CanMoveDownY(const float playerPos,
                      const float playerSize,
                      const float objPos,
                      const float objSize,
                      const float speed) const;
    /**
     * @brief Checks if the player Down movement get blocked by a object y coordinate texture size - collision size
     *
     * @param playerPos the current position of the player
     * @param objPos the object pos
     * @param objCollision the object collision size defined in data/world/worldTemplate.json -> TextureData last 2 entries
     * @param speed the player pixel movement distance
     * @return true if player can move Down or false if not
     */
    bool CanMoveDownY(const float playerPos, const float objPos, const uint8_t objCollision, const float speed) const;
};
