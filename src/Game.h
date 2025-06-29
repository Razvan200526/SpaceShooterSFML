#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

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
  void initWindow();
  void initCamera();

 public:
  // Constructor and destructors
  Game();
  ~Game();

  void upadtePollEvents();
  void update();
  void render();
  void run();

  const Player *getPlayer() { return player; }
};

#endif  // GAME_H
