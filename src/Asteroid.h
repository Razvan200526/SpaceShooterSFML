#ifndef ASTEROID_H
#define ASTEROID_H

#include "Player.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Asteroid{
public:
    sf::Texture texture;
    sf::Sprite sprite;
    int damage_to_player;
    std::string type;
private:
    Asteroid();
    ~Asteroid();
    void moveTowardsPlayer();
    void update();
};

#endif