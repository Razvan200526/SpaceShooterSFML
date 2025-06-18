#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

inline sf::Texture loadTexture(const std::string &filePath) {
  sf::Texture texture;
  if (!texture.loadFromFile(filePath)) {
    throw std::runtime_error("Failed to load texture from " + filePath);
  }
  return texture;
}

class Background {
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
  size_t level;

public:
  Background(size_t level);

  sf::Sprite getBackgroundSprite() const { return backgroundSprite; }
  void set_scale(sf::RenderWindow &window);
  size_t getLevel() const { return level; }
};
