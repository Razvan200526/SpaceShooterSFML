#ifndef PLAYER_H
#define PLAYER_H
#include "playerTexture.h"
#include <vector>
#include <iostream>

class Projectile;
class Asteroid;

class Player {
  private:
    sf::RenderWindow *window;
    sf::Sprite PlayerSprite;
    int health;
    sf::Vector2f position;
    sf::Vector2f velocity;
    std::vector<Projectile*> projectiles;
    // REMOVED: float dt; - We don't need to store dt in Player

  public:
    //constructor and destructor
    Player(sf::Vector2f startPos,sf::RenderWindow *w);
    ~Player();

    //utility functions
    void handleInput(float dt); // Pass dt as parameter
    void shoot();
    bool checkCollision();
    bool checkCollision(const Asteroid& asteroid);
    void update();

    //positional functions
    sf::Vector2f getPos() const;
    sf::Vector2f getVeloc() const ;
    sf::FloatRect getGlobalBounds() const ;
    const sf::Sprite& getSprite() const ;

    //setters
    void setPos(sf::Vector2f newPos);
    void setVelo(sf::Vector2f newVelocity);

    void move(sf::Vector2f offset);
    
};

#endif