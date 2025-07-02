#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>

// Forward declarations
class Player;
class Projectile;

class Enemy {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float health;
    float maxHealth;
    int damage_to_player;
    float shootCooldown;
    float lastShotTime;
    float detectionRange;
    float shootingRange;
    std::vector<Projectile*> projectiles;
    sf::RenderWindow* window;
    
public:
    // Constructor and destructor
    Enemy(sf::Vector2f startPos, sf::RenderWindow* window, int spriteType = 0);
    ~Enemy();
    
    // Core functions
    void update(float dt, const Player& player, const sf::View& view);
    void render(sf::RenderWindow& window) const;
    
    // Movement and AI
    void moveTowardsPlayer(const Player& player, float dt);
    void updateRotation(const Player& player);
    
    // Combat functions
    void shoot(const Player& player, float dt);
    void updateProjectiles(float dt);
    void removeProjectile(size_t index);
    bool canSeePlayer(const Player& player) const;
    bool isInShootingRange(const Player& player) const;
    bool isVisibleInView(const sf::View& view) const;
    
    // Collision and damage
    bool checkCollisionWithPlayer(const Player& player) const;
    void takeDamage(float damage);
    bool isDead() const;
    
    // Getters
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    const sf::Sprite& getSprite() const;
    const std::vector<Projectile*>& getProjectiles() const;
    float getHealth() const;
    int getDamage() const;
    
    // Setters
    void setPosition(sf::Vector2f pos);
    void setHealth(float hp);
};

#endif