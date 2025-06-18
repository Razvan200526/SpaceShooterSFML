#include "background.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
Background::Background(size_t level)
    : level(level),
      backgroundTexture(loadTexture("../textures/background.png")),
      backgroundSprite(backgroundTexture) {}

void Background::set_scale(sf::RenderWindow &window) {
  sf::Vector2u windowSize = window.getSize();
  float scaleX =
      static_cast<float>(windowSize.x) / backgroundTexture.getSize().x;
  float scaleY =
      static_cast<float>(windowSize.y) / backgroundTexture.getSize().y;
  backgroundSprite.setScale({scaleX, scaleY});

  window.draw(backgroundSprite);
}
