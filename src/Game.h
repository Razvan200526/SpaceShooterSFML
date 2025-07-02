#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <random>
#include <vector>

#include "Enemy.h"
#include "Player.h"
#include "chunk.h"

class Game {
 private:
  // variables
  sf::RenderWindow *window;
  ChunkManager chunkManager;
  sf::View camera;
  Player *player;
  sf::Clock deltaClock;
  float dt;
  bool isGameOver;
  std::optional<sf::Font> gameOverFont;
  std::optional<sf::Text> gameOverText;
  std::optional<sf::Text> restartText;
  
  // UI Elements
  std::optional<sf::Font> uiFont;
  std::optional<sf::Text> counterText;
  std::optional<sf::Text> healthText;
  std::optional<sf::RectangleShape> healthBarBackground;
  std::optional<sf::RectangleShape> healthBarForeground;
  int enemiesKilled;

  // Enemy management
  std::vector<Enemy *> enemies;
  float enemySpawnTimer;
  float enemySpawnInterval;
  std::mt19937 randomEngine;
  std::uniform_real_distribution<float> angleDist;
  std::uniform_real_distribution<float> distanceDist;
  std::uniform_int_distribution<int> spriteDist;

  void initWindow();
  void initCamera();
  void spawnEnemy();
  void updateEnemies();
  void renderEnemies();
  void checkCollisions();
  void resetGame();
  void handleGameOverInput();
  void renderGameOver();
  void initGameOverUI();
  void initUI();
  void updateUI();
  void renderUI();

 public:
  // Constructor and destructors
  Game();
  ~Game();

  void updatePollEvents();
  void update();
  void render();
  void run();
  void gameOver();

  const Player *getPlayer() { return player; }
};

#endif  // GAME_H
