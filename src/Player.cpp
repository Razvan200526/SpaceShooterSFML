#include "Player.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// Global texture MUST be initialized before any sprite uses it!
sf::Texture sharedPlayerTexture;
bool textureLoaded = false;

// Initialize texture before main() runs
struct TextureInitializer {
    TextureInitializer() {
        // Create a default texture so sprites can be constructed
        sf::Image defaultImage({16, 16}, sf::Color::White);
        sharedPlayerTexture.loadFromImage(defaultImage);
    }
} textureInit;

Player::Player(sf::Vector2f pos, sf::RenderWindow *window) 
    : position(pos), window(window), health(10), velocity({0.f, 0.f}), 
      PlayerSprite(sharedPlayerTexture) // Sprite MUST have valid texture
{
    if (!textureLoaded) {
        // Try to load the actual texture
        if (!sharedPlayerTexture.loadFromFile("purple.png")) {
            std::cerr << "Failed to load purple.png! Creating fallback..." << std::endl;
            
            // Create visible fallback texture
            sf::Image fallbackImage({16, 16});
            
            // Create a visible pattern
            for (unsigned int x = 0; x < 16; ++x) {
                for (unsigned int y = 0; y < 16; ++y) {
                    if ((x + y) % 2 == 0) {
                        fallbackImage.setPixel({x, y}, sf::Color::Red);
                    } else {
                        fallbackImage.setPixel({x, y}, sf::Color::Yellow);
                    }
                }
            }
            
            if(!sharedPlayerTexture.loadFromImage(fallbackImage)) {
                std::cerr << "Failed to create fallback texture!" << std::endl;
            }
        } else {
            std::cout << "Successfully loaded purple.png!" << std::endl;
        }
        
        // IMPORTANT: Disable smoothing for pixel art
        sharedPlayerTexture.setSmooth(false);
        textureLoaded = true;
    }
    
    // Update sprite's texture (it was using the default white texture)
    PlayerSprite.setTexture(sharedPlayerTexture);
    
    // Scale up significantly - 16x16 is tiny!
    PlayerSprite.setScale({0.8f, 0.8f}); // 128x128 pixels
    
    // Set origin to center
    sf::FloatRect bounds = PlayerSprite.getLocalBounds();
    PlayerSprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    
    // Set position
    PlayerSprite.setPosition(position);
    
    // Make sure it's visible
    PlayerSprite.setColor(sf::Color::White);
    
    // Debug
    std::cout << "\n=== PLAYER INITIALIZED ===" << std::endl;
    std::cout << "Position: " << position.x << ", " << position.y << std::endl;
    std::cout << "Texture size: " << sharedPlayerTexture.getSize().x << "x" << sharedPlayerTexture.getSize().y << std::endl;
    std::cout << "Sprite local bounds: " << bounds.size.x << "x" << bounds.size.y << std::endl;
    std::cout << "Scale: 8x8" << std::endl;
    std::cout << "Final size: " << bounds.size.x * 8 << "x" << bounds.size.y * 8 << " pixels" << std::endl;
    std::cout << "========================\n" << std::endl;
}

Player::~Player() = default;

void Player::update(){
    PlayerSprite.setPosition(position);
}

void Player::shoot(){
    // TODO
}

void Player::handleInput(float dt){
    velocity = {0.f, 0.f};

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        velocity.x = -300.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        velocity.x = 300.f;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        velocity.y = -300.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        velocity.y = 300.f;
    }

    position += velocity * dt;
}

sf::Vector2f Player::getPos() const{
    return position;
}

sf::Vector2f Player::getVeloc() const {
    return velocity;
}

sf::FloatRect Player::getGlobalBounds() const {
    return PlayerSprite.getGlobalBounds();
}

const sf::Sprite& Player::getSprite() const {
    return PlayerSprite;
}