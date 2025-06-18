#include "background.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

void handleInput(sf::CircleShape &s, float speed) {
  float dx = 0.f, dy = 0.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    dy -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    dy += speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    dx -= speed;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    dx += speed;
  s.move({dx, dy});
}
int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Game");
  Background background(1);
  sf::CircleShape s(25.f);
  s.setFillColor(sf::Color::Red);
  s.setPosition({300, 300});
  sf::View view = window.getView();
  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    handleInput(s, 0.2f);
    view.setCenter(s.getPosition());
    window.setView(view);
    window.clear();
    window.draw(background.getBackgroundSprite());
    background.set_scale(window);
    window.draw(s);
    window.display();
  }

  return 0;
}
