#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <vector>

class Projectile;
class Asteroid;

class Player {
 private:
  sf::RenderWindow* window;
  sf::Sprite PlayerSprite;
  float health;
  float maxHealth;
  sf::Vector2f position;
  sf::Vector2f velocity;
  std::vector<Projectile*> projectiles;
  float shootCooldown;
  float lastShotTime;
  float currentRotation;
  float targetRotation;
  float rotationSpeed;
  float lastCollisionTime;
  float collisionCooldown;

 public:
  // constructor and destructor
  Player(sf::Vector2f startPos, sf::RenderWindow* w);
  ~Player();

  // utility functions
  void handleInput(float dt);
  void shoot();
  void updateProjectiles(float dt);
  void updateRotation(float dt);
  void takeDamage(float damage);
  bool canTakeCollisionDamage();
  bool isDead() const;
  bool checkCollision();
  void update();
  void removeProjectiles(size_t projIndex);
  // positional functions
  sf::Vector2f getPos() const;
  sf::Vector2f getVeloc() const;
  sf::FloatRect getGlobalBounds() const;
  const sf::Sprite& getSprite() const;
  const std::vector<Projectile*>& getProjectiles() const;
  float getHealth() const;
  float getMaxHealth() const;

  // setters
  void setPos(sf::Vector2f newPos);
  void setVelo(sf::Vector2f newVelocity);

  void move(sf::Vector2f offset);
};

#endif
