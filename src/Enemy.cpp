#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <exception>
#include <string>

sf::Texture sharedEnemyTextures[3];
bool enemyTexturesLoaded = false;

struct EnemyTextureInitializer {
    EnemyTextureInitializer() {
        std::string filenames[] = {"Gray1.png", "Gray2.png", "Gray3.png"};
        
        for(int i = 0; i < 3; i++) {
            if (!sharedEnemyTextures[i].loadFromFile(filenames[i])) {
               std::throw_with_nested("error");
            }
            sharedEnemyTextures[i].setSmooth(false);
        }
        enemyTexturesLoaded = true;
    }
} enemyTextureInit;

Enemy::Enemy(sf::Vector2f startPos, sf::RenderWindow* window, int spriteType)
    : sprite(sharedEnemyTextures[0]),
      position(startPos),
      velocity({0.0f, 0.0f}),
      speed(200.0f),
      health(50.0f),
      maxHealth(50.0f),
      damage_to_player(10),
      shootCooldown(0.8f),
      lastShotTime(0.0f),
      detectionRange(500.0f),
      shootingRange(100.0f),
      window(window)
{
    spriteType = std::max(0, std::min(2, spriteType));
    
    sprite.setTexture(sharedEnemyTextures[spriteType]);
    sprite.setScale({0.8f, 0.8f});
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    sprite.setPosition(position);
}

Enemy::~Enemy() {
    for(Projectile* projectile : projectiles) {
        delete projectile;
    }
    projectiles.clear();
}

void Enemy::update(float dt, const Player& player, const sf::View& view) {
    lastShotTime += dt;
    if(lastShotTime > shootCooldown) {
        lastShotTime = shootCooldown;
    }
    
    updateRotation(player);
    
    if (isInShootingRange(player) && isVisibleInView(view)) {
        moveTowardsPlayer(player, dt);
        velocity *= 0.3f;
        shoot(player, dt);
    } else {
        moveTowardsPlayer(player, dt);
    }
    
    position += velocity * dt;
    sprite.setPosition(position);
    
    updateProjectiles(dt);
}

void Enemy::render(sf::RenderWindow& window) const {
    window.draw(sprite);
    
    for(const Projectile* projectile : projectiles) {
        window.draw(projectile->getSprite());
    }
}

void Enemy::moveTowardsPlayer(const Player& player, float /*dt*/) {
    sf::Vector2f playerPos = player.getPos();
    sf::Vector2f direction = playerPos - position;
    
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance > 0.0f) {
        velocity.x = (direction.x / distance) * speed;
        velocity.y = (direction.y / distance) * speed;
    }
}

void Enemy::updateRotation(const Player& player) {
    sf::Vector2f playerPos = player.getPos();
    sf::Vector2f direction = playerPos - position;
    
    if (direction.x != 0.0f || direction.y != 0.0f) {
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
        angle += 90.0f;
        sprite.setRotation(sf::degrees(angle));
    }
}

void Enemy::shoot(const Player& player, float /*dt*/) {
    if (lastShotTime >= shootCooldown) {
        sf::Vector2f playerPos = player.getPos();
        sf::Vector2f direction = playerPos - position;
        
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 0.0f) {
            direction.x /= distance;
            direction.y /= distance;
            
            // Calculate spawn position at front of enemy sprite
            float spawnOffset = 25.0f;
            sf::Vector2f spawnPosition = position + direction * spawnOffset;
            
            projectiles.push_back(new Projectile(spawnPosition, direction));
            lastShotTime = 0.0f;
        }
    }
}

void Enemy::updateProjectiles(float dt) {
    for(size_t i = 0; i < projectiles.size(); i++) {
        projectiles[i]->move(dt);
        if(projectiles[i]->checkOutOfBounds(window)) {
            delete projectiles[i];
            projectiles.erase(projectiles.begin() + i);
            i--; // Adjust index after erasing
        }
    }
}

void Enemy::removeProjectile(size_t index) {
    if (index < projectiles.size()) {
        delete projectiles[index];
        projectiles.erase(projectiles.begin() + index);
    }
}

bool Enemy::canSeePlayer(const Player& player) const {
    sf::Vector2f playerPos = player.getPos();
    sf::Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    return distance <= detectionRange;
}

bool Enemy::isInShootingRange(const Player& player) const {
    sf::Vector2f playerPos = player.getPos();
    sf::Vector2f direction = playerPos - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    return distance * 1.5f <= shootingRange;
}

bool Enemy::isVisibleInView(const sf::View& view) const {
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    
    // Calculate view bounds
    float left = viewCenter.x - viewSize.x / 2.0f;
    float right = viewCenter.x + viewSize.x / 2.0f;
    float top = viewCenter.y - viewSize.y / 2.0f;
    float bottom = viewCenter.y + viewSize.y / 2.0f;
    
    // Add some margin for enemies that are just outside the view
    float margin = 100.0f;
    left -= margin;
    right += margin;
    top -= margin;
    bottom += margin;
    
    // Check if enemy position is within the extended view bounds
    return (position.x >= left && position.x <= right && 
            position.y >= top && position.y <= bottom);
}

bool Enemy::checkCollisionWithPlayer(const Player& player) const {
    sf::FloatRect enemyBounds = sprite.getGlobalBounds();
    sf::FloatRect playerBounds = player.getGlobalBounds();
    
    return (enemyBounds.position.x < playerBounds.position.x + playerBounds.size.x &&
            enemyBounds.position.x + enemyBounds.size.x > playerBounds.position.x &&
            enemyBounds.position.y < playerBounds.position.y + playerBounds.size.y &&
            enemyBounds.position.y + enemyBounds.size.y > playerBounds.position.y);
}

void Enemy::takeDamage(float damage) {
    health -= damage;
    if (health < 0.0f) {
        health = 0.0f;
    }
}

bool Enemy::isDead() const {
    return health <= 0.0f;
}

// Getters
sf::Vector2f Enemy::getPosition() const {
    return position;
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

const sf::Sprite& Enemy::getSprite() const {
    return sprite;
}

const std::vector<Projectile*>& Enemy::getProjectiles() const {
    return projectiles;
}

float Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage_to_player;
}

// Setters
void Enemy::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(position);
}

void Enemy::setHealth(float hp) {
    health = hp;
    if (health > maxHealth) {
        health = maxHealth;
    }
}
