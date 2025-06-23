#include "chunk.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

sf::Vector2i ChunkManager::worldToChunk(sf::Vector2f worldPos) {
  return sf::Vector2i(static_cast<int>(std::floor(worldPos.x / chunkSize)),
                      static_cast<int>(std::floor(worldPos.y / chunkSize)));
}

void ChunkManager::drawChunks(sf::RenderWindow &window) {
  for (const auto &chunk : loadedChunks) {
    if (chunk.isLoaded) {
      window.draw(chunk.backgroundSprite);
    }
  }
}

void ChunkManager::UpdateChunks(sf::Vector2f pos) {
  sf::Vector2i currentChunk = worldToChunk(pos);
  std::vector<sf::Vector2i> chunksToLoad;

  int totalChunks = (2 * load_distance + 1) * (2 * load_distance + 1);
  chunksToLoad.reserve(totalChunks);
  for (int x = -load_distance; x <= load_distance; ++x) {
    for (int y = -load_distance; y <= load_distance; ++y) {
      sf::Vector2i chunkPos(currentChunk.x + x, currentChunk.y + y);
      chunksToLoad.push_back(chunkPos);
    }
  }

  // Load new chunks
  for (const auto &chunkPos : chunksToLoad) {
    auto it = std::find_if(
        loadedChunks.begin(), loadedChunks.end(),
        [&chunkPos](const Chunk &c) { return c.position == chunkPos; });
    if (it == loadedChunks.end()) {
      loadedChunks.emplace_back(chunkPos);
      loadedChunks.back().load();
    }
  }

  // Unload chunks that are no longer needed
  for (auto it = loadedChunks.begin(); it != loadedChunks.end();) {
    if (std::find(chunksToLoad.begin(), chunksToLoad.end(), it->position) ==
        chunksToLoad.end()) {
      it->unload();
      it = loadedChunks.erase(it);
    } else {
      ++it;
    }
  }
}

// Shared texture to avoid loading the same texture multiple times
static sf::Texture sharedBackgroundTexture;
static bool textureLoaded = false;

Chunk::Chunk(sf::Vector2i pos) : position(pos) , backgroundSprite(sharedBackgroundTexture) {
  // Load shared texture only once
  if (!textureLoaded) {
    if (!sharedBackgroundTexture.loadFromFile("background.png")) {
      std::cerr << "Failed to load shared background texture!" << std::endl;
    } else {
      textureLoaded = true;
      std::cout << "Shared background texture loaded successfully" << std::endl;
    }
  }

  // Set the sprite to use the shared texture
  backgroundSprite.setScale({1.5f,1.5f});
  backgroundSprite.setTexture(sharedBackgroundTexture);
}

void Chunk::load() {
  if (isLoaded)
    return;

  try {
    // Check if shared texture is available
    if (!textureLoaded) {
      throw std::runtime_error("Shared texture not loaded");
    }

    std::cout << "Loading chunk at " << position.x << ", " << position.y << std::endl;

    // Calculate world position using consistent chunk size (600)
    float worldX = position.x * CHUNK_SIZE;  // Use chunkSize from ChunkManager
    float worldY = position.y * CHUNK_SIZE;
    backgroundSprite.setPosition({worldX, worldY});

    // Scale sprite to fit chunk size
    sf::Vector2u textureSize = sharedBackgroundTexture.getSize();
    float scaleX = 600.f / textureSize.x;  // Use consistent chunk size
    float scaleY = 600.f / textureSize.y;
    backgroundSprite.setScale({scaleX, scaleY});

    isLoaded = true;
    std::cout << "Successfully loaded chunk at " << position.x << ", " << position.y 
              << " at world position (" << worldX << ", " << worldY << ")" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error loading chunk at " << position.x << ", " << position.y
              << ": " << e.what() << std::endl;
    isLoaded = false;  // Ensure it's marked as not loaded on failure
  }
}

void Chunk::unload() {
  if (!isLoaded)
    return;

  std::cout << "Unloading chunk at " << position.x << ", " << position.y << std::endl;
  isLoaded = false;
  // Note: We don't reset the sprite texture since we're using a shared texture
}