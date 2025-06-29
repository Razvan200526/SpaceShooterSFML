
#include "Game.h"
#include "Projectile.h"

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

void Game::upadtePollEvents() {
  while (const std::optional event = this->window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      this->window->close();
    }
  }
}

void Game::render() {
  this->window->clear();

  this->window->setView(camera);

  chunkManager.drawChunks(*this->window);

  const sf::Sprite& playerSprite = player->getSprite();
  this->window->draw(playerSprite);

  // Render projectiles
  const std::vector<Projectile*>& projectiles = player->getProjectiles();
  for(const Projectile* projectile : projectiles) {
    this->window->draw(projectile->getSprite());
  }

  this->window->display();
}

void Game::update() {
  this->upadtePollEvents();

  player->handleInput(dt);
  player->update();
  camera.setCenter(player->getPos());
  chunkManager.UpdateChunks(player->getPos());
}

Game::Game() {
  this->initWindow();

  dt = 0.0f;
  deltaClock.restart();

  player = new Player({400.f, 300.f}, this->window);
  this->initCamera();
}
