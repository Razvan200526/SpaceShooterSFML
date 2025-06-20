#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Asteroid.h"
class Projectile{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int damage;
public:
    Projectile();
    ~Projectile();
    //utility functions
    void move();//this will be used to shoot the projectile when shooted by the player
    bool CheckColission(const Asteroid* other);
};

#endif