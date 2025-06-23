#include "Game.h"
#include <iostream>
#include <exception>

int main() {
  try {
    Game game;
    game.run();
  } catch (const std::system_error& e) {
    std::cerr << "System error: " << e.what() << std::endl;
    std::cerr << "Error code: " << e.code() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown exception occurred" << std::endl;
    return 1;
  }
  
  return 0;
}
