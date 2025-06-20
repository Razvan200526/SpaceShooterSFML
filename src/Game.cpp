#include "Game.h"
#include <exception>
#include <iostream>

void Game::initWindow() {
  this->window =
      new sf::RenderWindow(sf::VideoMode({800, 600}), "SpaceShooter");
}

void Game::initCamera() {
  camera.setSize({800.f, 600.f});
  camera.setCenter(player->getPos());
}
Game::~Game() { 
  delete this->window;
  delete this->player;
}

void Game::run() {

  while (this->window->isOpen()) {
    this->update();
    this->render();
  }
}

void Game::upadtePollEvents() {

  while (const std::optional event = this->window->pollEvent()) {
    dt = deltaClock.getElapsedTime().asSeconds();

    if (event->is<sf::Event::Closed>()) {
      this->window->close();
    }
    //this->player->update();
  }
}

void Game::render() {
  this->window->clear();


  // Render game objects here
  chunkManager.UpdateChunks(player->getPos());
  this->window->draw(player->getSprite());
  this->window->display();
}

void Game::update() {
  this->upadtePollEvents();

  // Update game objects here
}

Game::Game(){
  this->initWindow(); // make the window
  player = new Player({0.f,0.f},this->window,this->dt);
  this->initCamera();
  try {
    render();
  }catch(std::exception& e){
    std::cerr << "Could not render the game";
    exit(1);
  }
}
