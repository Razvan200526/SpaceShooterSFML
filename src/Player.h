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
    float dt;

  public:
    //constructor and destructor
    Player(sf::Vector2f startPos,sf::RenderWindow *w,float dt);
    ~Player();

    //void render(); //idk if i need this yet beacause in the game i can just window->draw(player.getSprite())
    //utility functions
    void handleInput();
    void shoot();
    bool checkCollision();
    bool checkCollision(const Asteroid& asteroid);
    void update(); //basically i'll call checkcollison with the window and the asteroids and update accordingly

    //positional functions
    sf::Vector2f getPos() const;
    sf::Vector2f getVeloc() const ;
    sf::FloatRect getGlobalBounds() const ;
    sf::Sprite getSprite() const ;

    //setters
    void setPos(sf::Vector2f newPos);
    void setVelo(sf::Vector2f newVelocity);

    void move(sf::Vector2f offset);
    
};

#endif