#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include "chunk.h"
#include "Player.h"
class Game {

private:
  // variables
  sf::RenderWindow *window;
  ChunkManager chunkManager;
  sf::View camera;
  Player *player;
  sf::Clock deltaClock;//used for constant movement across all platforms
  float dt;
  // functions
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

  const Player* getPlayer(){return player;}
};

#endif // GAME_H
