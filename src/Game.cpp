#include "Game.h"
#include "Projectile.h"
#include "Enemy.h"
#include <cmath>
#include <random>
#include <optional>
#include <string>

void Game::initWindow() {
  this->window =
      new sf::RenderWindow(sf::VideoMode({1920, 1080}), "SpaceShooter");
}

void Game::initCamera() {
  camera.setSize({1920.f, 1080.f});
  camera.setCenter(player->getPos());
  camera.zoom(0.1f);
  this->window->setView(camera);
}

Game::~Game() {
  if (this->window && this->window->isOpen()) {
    this->window->close();
  }

  delete this->player;
  this->player = nullptr;
  
  for(Enemy* enemy : enemies) {
    delete enemy;
  }
  enemies.clear();

  delete this->window;
  this->window = nullptr;
}

void Game::run() {
  while (this->window->isOpen()) {
    dt = deltaClock.restart().asSeconds();

    this->update();
    this->render();
  }
}

void Game::updatePollEvents() {
  while (const std::optional event = this->window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      this->window->close();
    }
    
    if (isGameOver) {
      if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
          this->window->close();
        } else if (keyPressed->code == sf::Keyboard::Key::R) {
          resetGame();
        }
      }
    }
  }
}

void Game::render() {
  this->window->clear();

  this->window->setView(camera);

  chunkManager.drawChunks(*this->window);

  const sf::Sprite& playerSprite = player->getSprite();
  this->window->draw(playerSprite);

  const std::vector<Projectile*>& projectiles = player->getProjectiles();
  for(const Projectile* projectile : projectiles) {
    this->window->draw(projectile->getSprite());
  }
  
  renderEnemies();
  renderUI();
  renderGameOver();

  this->window->display();
}

void Game::renderEnemies() {
  for(const Enemy* enemy : enemies) {
    enemy->render(*window);
  }
}

void Game::update() {
  this->updatePollEvents();
  
  if(player->isDead() && !isGameOver) {
    gameOver();
    return;
  }
  
  if(isGameOver) {
    return;
  }

  player->handleInput(dt);
  player->update();
  camera.setCenter(player->getPos());
  chunkManager.UpdateChunks(player->getPos());
  
  enemySpawnTimer += dt;
  if(enemySpawnTimer >= enemySpawnInterval) {
    spawnEnemy();
    enemySpawnTimer = 0.0f;
  }
  
  updateEnemies();
  updateUI();
  checkCollisions();
}

void Game::spawnEnemy() {
  float angle = angleDist(randomEngine);
  float distance = distanceDist(randomEngine);
  
  sf::Vector2f playerPos = player->getPos();
  sf::Vector2f enemyPos;
  enemyPos.x = playerPos.x + std::cos(angle) * distance;
  enemyPos.y = playerPos.y + std::sin(angle) * distance;
  
  int spriteType = spriteDist(randomEngine);
  enemies.push_back(new Enemy(enemyPos, window, spriteType));
}

void Game::updateEnemies() {
  for(size_t i = 0; i < enemies.size(); i++) {
    enemies[i]->update(dt, *player, camera);
    
    if(enemies[i]->isDead()) {
      delete enemies[i];
      enemies.erase(enemies.begin() + i);
      i--;
    }
  }
}

void Game::checkCollisions() {
  const std::vector<Projectile*>& playerProjectiles = player->getProjectiles();
  for(size_t projIndex = 0; projIndex < playerProjectiles.size(); projIndex++) {
    sf::FloatRect projectileBounds = playerProjectiles[projIndex]->getSprite().getGlobalBounds();
    
    for(size_t enemyIndex = 0; enemyIndex < enemies.size(); enemyIndex++) {
      sf::FloatRect enemyBounds = enemies[enemyIndex]->getGlobalBounds();
      
      if (projectileBounds.position.x < enemyBounds.position.x + enemyBounds.size.x &&
          projectileBounds.position.x + projectileBounds.size.x > enemyBounds.position.x &&
          projectileBounds.position.y < enemyBounds.position.y + enemyBounds.size.y &&
          projectileBounds.position.y + projectileBounds.size.y > enemyBounds.position.y) {
        
        enemies[enemyIndex]->takeDamage(25.0f);
        
        if (enemies[enemyIndex]->isDead()) {
          enemiesKilled++;
        }
        
        break;
      }
    }
  }
  
  for(Enemy* enemy : enemies) {
    const std::vector<Projectile*>& enemyProjectiles = enemy->getProjectiles();
    sf::FloatRect playerBounds = player->getGlobalBounds();
    
    for(size_t projIndex = 0; projIndex < enemyProjectiles.size(); projIndex++) {
      sf::FloatRect projectileBounds = enemyProjectiles[projIndex]->getSprite().getGlobalBounds();
      
      if (projectileBounds.position.x < playerBounds.position.x + playerBounds.size.x &&
          projectileBounds.position.x + projectileBounds.size.x > playerBounds.position.x &&
          projectileBounds.position.y < playerBounds.position.y + playerBounds.size.y &&
          projectileBounds.position.y + projectileBounds.size.y > playerBounds.position.y) {
        
        player->takeDamage(enemy->getDamage());
        
        enemy->removeProjectile(projIndex);
        break;
      }
    }
  }
  
  for(Enemy* enemy : enemies) {
    if (enemy->checkCollisionWithPlayer(*player) && player->canTakeCollisionDamage()) {
      player->takeDamage(enemy->getDamage());
    }
  }
}

void Game::gameOver() {
  isGameOver = true;
}

void Game::initGameOverUI() {
  gameOverFont = sf::Font();
  if (!gameOverFont->openFromFile("ARCADECLASSIC.TTF")) {
    gameOverFont = std::nullopt;
    return;
  }
  
  gameOverText = sf::Text(gameOverFont.value());
  restartText = sf::Text(gameOverFont.value());
  
  gameOverText->setString("GAME OVER");
  gameOverText->setCharacterSize(24);
  gameOverText->setFillColor(sf::Color::Red);
  
  restartText->setString("Press R to Restart or ESC to Quit");
  restartText->setCharacterSize(12);
  restartText->setFillColor(sf::Color::White);
}

void Game::handleGameOverInput() {
  while (std::optional<sf::Event> event = window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window->close();
    }
    
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape) {
        window->close();
      } else if (keyPressed->code == sf::Keyboard::Key::R) {
        resetGame();
      }
    }
  }
}

void Game::resetGame() {
  delete player;
  player = new Player({400.f, 300.f}, this->window);
  
  for(Enemy* enemy : enemies) {
    delete enemy;
  }
  enemies.clear();
  
  enemySpawnTimer = 0.0f;
  deltaClock.restart();
  
  isGameOver = false;
  enemiesKilled = 0;
  
  camera.setCenter(player->getPos());
}

void Game::renderGameOver() {
  if (!isGameOver || !gameOverText.has_value() || !restartText.has_value()) {
    return;
  }
  
  sf::Vector2f viewCenter = camera.getCenter();
  
  sf::FloatRect gameOverBounds = gameOverText->getLocalBounds();
  gameOverText->setPosition(sf::Vector2f(
    viewCenter.x - gameOverBounds.size.x / 2.0f,
    viewCenter.y - gameOverBounds.size.y / 2.0f - 50.0f
  ));
  
  sf::FloatRect restartBounds = restartText->getLocalBounds();
  restartText->setPosition(sf::Vector2f(
    viewCenter.x - restartBounds.size.x / 2.0f,
    viewCenter.y + 50.0f
  ));
  
  window->draw(gameOverText.value());
  window->draw(restartText.value());
}

void Game::initUI() {
  uiFont = sf::Font();
  if (!uiFont->openFromFile("ARCADECLASSIC.TTF")) {
    uiFont = std::nullopt;
    return;
  }
  
  counterText = sf::Text(uiFont.value());
  counterText->setString("Score: 0");
  counterText->setCharacterSize(20);
  counterText->setFillColor(sf::Color::White);
  
  healthText = sf::Text(uiFont.value());
  healthText->setCharacterSize(16);
  healthText->setFillColor(sf::Color::White);
  
  healthBarBackground = sf::RectangleShape();
  healthBarBackground->setSize(sf::Vector2f(150.0f, 15.0f));
  healthBarBackground->setFillColor(sf::Color::Red);
  healthBarBackground->setOutlineThickness(1.0f);
  healthBarBackground->setOutlineColor(sf::Color::White);
  
  healthBarForeground = sf::RectangleShape();
  healthBarForeground->setSize(sf::Vector2f(150.0f, 15.0f));
  healthBarForeground->setFillColor(sf::Color::Green);
}

void Game::updateUI() {
  if (!counterText.has_value() || !healthText.has_value() || 
      !healthBarBackground.has_value() || !healthBarForeground.has_value()) {
    return;
  }
  
  counterText->setString("Score: " + std::to_string(enemiesKilled));
  
  float currentHealth = player->getHealth();
  float maxHealth = player->getMaxHealth();
  healthText->setString("Health: " + std::to_string(static_cast<int>(currentHealth)) + 
                       "/" + std::to_string(static_cast<int>(maxHealth)));
  
  float healthPercentage = currentHealth / maxHealth;
  if (healthPercentage < 0.0f) healthPercentage = 0.0f;
  healthBarForeground->setSize(sf::Vector2f(150.0f * healthPercentage, 15.0f));
  
  if (healthPercentage > 0.6f) {
    healthBarForeground->setFillColor(sf::Color::Green);
  } else if (healthPercentage > 0.3f) {
    healthBarForeground->setFillColor(sf::Color::Yellow);
  } else {
    healthBarForeground->setFillColor(sf::Color::Red);
  }
}

void Game::renderUI() {
  if (!counterText.has_value() || !healthText.has_value() || 
      !healthBarBackground.has_value() || !healthBarForeground.has_value()) {
    return;
  }
  
  sf::View defaultView = window->getDefaultView();
  window->setView(defaultView);
  
  sf::Vector2f screenSize = defaultView.getSize();
  
  sf::FloatRect counterBounds = counterText->getLocalBounds();
  counterText->setPosition(sf::Vector2f(
    screenSize.x - counterBounds.size.x - 20.0f,
    20.0f
  ));
  
  float bottomY = screenSize.y - 40.0f;
  float rightX = screenSize.x - 170.0f;
  
  healthText->setPosition(sf::Vector2f(rightX, bottomY - 25.0f));
  
  healthBarBackground->setPosition(sf::Vector2f(rightX, bottomY));
  healthBarForeground->setPosition(sf::Vector2f(rightX, bottomY));
  
  window->draw(counterText.value());
  window->draw(healthText.value());
  window->draw(healthBarBackground.value());
  window->draw(healthBarForeground.value());
  
  window->setView(camera);
}

Game::Game() {
  this->initWindow();

  dt = 0.0f;
  deltaClock.restart();

  player = new Player({400.f, 300.f}, this->window);
  this->initCamera();
  
  isGameOver = false;
  initGameOverUI();
  
  enemiesKilled = 0;
  initUI();
  
  enemySpawnTimer = 0.0f;
  enemySpawnInterval = 1.0f;
  randomEngine.seed(std::random_device{}());
  angleDist = std::uniform_real_distribution<float>(0.0f, 2.0f * 3.14159f);
  distanceDist = std::uniform_real_distribution<float>(200.0f, 300.0f);
  spriteDist = std::uniform_int_distribution<int>(0, 2);
}
