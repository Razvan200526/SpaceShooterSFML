#include "Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

sf::Texture sharedPlayerTexture;
bool textureLoaded = false;

Player::Player(sf::Vector2f pos,sf::RenderWindow *window,float dt) : position(pos), PlayerSprite(sharedPlayerTexture) , window(window) 
, health(10) , velocity({100.f,100.f}) , dt(dt)
{
    if (!textureLoaded) {
        if (!sharedPlayerTexture.loadFromFile("ships/Blue.png")) {
            std::cout << "Failed to load player texture!" << std::endl;
        }
        textureLoaded = true;
    }
    PlayerSprite.setTexture(sharedPlayerTexture);
    PlayerSprite.setPosition(position);

    //have to initialize projectile vector
}

Player::~Player() = default;

void Player::update(){
    // if(checkCollision()){
    //   float middle_x = static_cast<float>(this->window->getSize().x) / 2.0f;
    //   float middle_y = static_cast<float>(this->window->getSize().y) / 2.0f;
    //   position = { middle_x,middle_y };
    // }
    //check collision with asteroid
}

void Player::shoot(){
    // if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
    //     //projectiles.push_back(new Projectile())
    //     //projectile.move() have to implement logic to kind of track asteroids;

    //     // for(auto proj : projectiles){
    //     //     if(proj->CheckCollision() || proj->CheckOutOfBounds){
    //     //         delete proj;
    //     //     }
    //     // }
    // }
}

void Player::handleInput(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        velocity.x = -200.f;
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        velocity.x = 200.f;
    }else {
        velocity.x = 0.f;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        // Move up
        velocity.y = -200.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        // Move down
        velocity.y = 200.f;
    }
    else {
        velocity.y = 0.f;
    }

    position = velocity * dt;

    PlayerSprite.setPosition(position);
}


sf::Vector2f Player::getPos() const{
    return this->position;
}

sf::Vector2f Player::getVeloc() const {
    return velocity;
}

sf::FloatRect Player::getGlobalBounds() const {
    return PlayerSprite.getGlobalBounds();
}

sf::Sprite Player::getSprite() const {
    return PlayerSprite;
}