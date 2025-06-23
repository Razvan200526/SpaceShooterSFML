#include "Game.h"
#include <SFML/Graphics/Color.hpp>
#include <iostream>

void Game::initWindow() {
  this->window =
      new sf::RenderWindow(sf::VideoMode({800, 600}), "SpaceShooter");
  this->window->setFramerateLimit(60);
}

void Game::initCamera() {
  camera.setSize({800.f, 600.f});
  camera.setCenter(player->getPos());
  // Don't zoom for now - let's see everything clearly
  camera.zoom(0.2f);
  this->window->setView(camera);
}

Game::~Game() { 
  delete this->window;
  delete this->player;
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
  // Clear with black
  this->window->clear(sf::Color::Black);

  // Apply camera view
  this->window->setView(camera);
  
  // Draw chunks as background
  chunkManager.drawChunks(*this->window);
  
  // Draw the player sprite on top
  const sf::Sprite& playerSprite = player->getSprite();
  this->window->draw(playerSprite);
  
  this->window->display();
}

void Game::update() {
  this->upadtePollEvents();

  player->handleInput(dt);
  player->update();
  camera.setCenter(player->getPos());
  chunkManager.UpdateChunks(player->getPos());
}

Game::Game(){
  this->initWindow();
  
  dt = 0.0f;
  deltaClock.restart();
  
  // Create player at center
  player = new Player({400.f, 300.f}, this->window);
  this->initCamera();
}