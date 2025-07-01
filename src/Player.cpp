#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cmath>

#include "Player.h"
#include "Projectile.h"

sf::Texture sharedPlayerTexture;
bool textureLoaded = false;

struct TextureInitializer {
  TextureInitializer() {
    sf::Image defaultImage(sf::Vector2u{16, 16}, sf::Color::White);
    if(!sharedPlayerTexture.loadFromImage(defaultImage)){
      std::cout << "Couldn't load default image\n";
    }
  }
} textureInit;

Player::Player(sf::Vector2f pos, sf::RenderWindow* window)
    : window(window),
      PlayerSprite(sharedPlayerTexture),
      health(10),
      position(pos),
      velocity({0.f, 0.f}),
      shootCooldown(0.07f), 
      lastShotTime(0.0f),
      currentRotation(0.0f),
      targetRotation(0.0f),
      rotationSpeed(500.0f) 
{
  if (!textureLoaded) {
    if (!sharedPlayerTexture.loadFromFile("purple.png")) {
      std::cerr << "Failed to load purple.png!"
                << std::endl;

      sharedPlayerTexture.setSmooth(false);
      textureLoaded = true;
    }

    PlayerSprite.setTexture(sharedPlayerTexture);

    PlayerSprite.setScale({0.8f, 0.8f});

    sf::FloatRect bounds = PlayerSprite.getLocalBounds();
    PlayerSprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});

    PlayerSprite.setPosition(position);
  }
}
Player::~Player() = default;

void Player::update() { 
  PlayerSprite.setPosition(position);
  PlayerSprite.setRotation(sf::degrees(currentRotation));
}

void Player::handleInput(float dt) {
  velocity = {0.f, 0.f};

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
    velocity.x = -300.f;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    velocity.x = 300.f;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
    velocity.y = -300.f;
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
    velocity.y = 300.f;
  }

  sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
  sf::Vector2f worldMousePos = window->mapPixelToCoords(mousePos);
  
  sf::Vector2f direction = worldMousePos - position;
  if (direction.x != 0.0f || direction.y != 0.0f) {
    targetRotation = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    targetRotation += 90.0f;
  }

  position += velocity * dt;
  
  updateRotation(dt);
  
  lastShotTime += dt;
  if(lastShotTime > shootCooldown) {
    lastShotTime = shootCooldown;
  }
  
  shoot();
  updateProjectiles(dt);
}

sf::Vector2f Player::getPos() const { return position; }

sf::Vector2f Player::getVeloc() const { return velocity; }

sf::FloatRect Player::getGlobalBounds() const {
  return PlayerSprite.getGlobalBounds();
}

const sf::Sprite& Player::getSprite() const { return PlayerSprite; }


void Player::shoot(){
  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
    if(lastShotTime >= shootCooldown){
      float angleRad = (currentRotation - 90.0f) * 3.14159f / 180.0f;
      sf::Vector2f shootDirection(std::cos(angleRad), std::sin(angleRad));
      
      float spawnOffset = 5.0f; 
      sf::Vector2f spawnPosition = position + shootDirection * spawnOffset;
      
      projectiles.push_back(new Projectile(spawnPosition, shootDirection));
      lastShotTime = 0.0f;
    }
  }
}

void Player::updateProjectiles(float dt){
  for(size_t i = 0; i < projectiles.size(); i++){
    projectiles[i]->move(dt);
    if(projectiles[i]->checkOutOfBounds(window)){
      delete projectiles[i];
      projectiles.erase(projectiles.begin() + i);
      i--; 
    }
  }
}

void Player::updateRotation(float dt) {

  float angleDiff = targetRotation - currentRotation;
  
  while (angleDiff > 180.0f) angleDiff -= 360.0f;
  while (angleDiff < -180.0f) angleDiff += 360.0f;
  
  if (std::abs(angleDiff) > 0.1f) { 
    float rotationStep = rotationSpeed * dt;
    if (std::abs(angleDiff) < rotationStep) {
      currentRotation = targetRotation;
    } else {
      currentRotation += (angleDiff > 0 ? rotationStep : -rotationStep);
    }
    

    while (currentRotation >= 360.0f) currentRotation -= 360.0f;
    while (currentRotation < 0.0f) currentRotation += 360.0f;
  
    PlayerSprite.setRotation(sf::degrees(currentRotation));
  }
}

const std::vector<Projectile*>& Player::getProjectiles() const {
  return projectiles;
}