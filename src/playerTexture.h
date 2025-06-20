#pragma once
#include <SFML/Graphics.hpp>

inline void loadPlayerTexture(sf::Texture playerTexture) {
  if (!playerTexture.loadFromFile("ships/purple.png")) {
    throw std::runtime_error("Failed to load player texture");
  }
}
