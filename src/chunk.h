#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
constexpr int CHUNK_SIZE = 600;

struct Chunk{
  sf::Vector2i position;
  sf::Sprite
      backgroundSprite; // Removed texture since we're using shared texture
  bool isLoaded = false;

  Chunk(sf::Vector2i pos);
  void load();
  void unload();
};

class ChunkManager {
  std::vector<Chunk> loadedChunks;
  const int chunkSize = CHUNK_SIZE;
  const int load_distance = 2;

public:
  void UpdateChunks(sf::Vector2f pos);
  void drawChunks(sf::RenderWindow &window);
  sf::Vector2i worldToChunk(sf::Vector2f pos);
};
