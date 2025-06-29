#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
#include "Asteroid.h"
class Projectile{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    int damage;
    sf::Vector2f pos;
    sf::Vector2f velocity;
public:
    Projectile(sf::Vector2f pos, sf::Vector2f direction);
    ~Projectile();
    //utility functions
    void move(float dt);//this will be used to move the projectile each frame
    bool CheckColission(const Asteroid* other);
    bool checkOutOfBounds(sf::RenderWindow *window);
    const sf::Sprite& getSprite() const;
};

#endif