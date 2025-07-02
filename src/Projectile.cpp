#include "Projectile.h"

sf::Texture sharedProjTexture;
bool loaded = false;


struct TextureInitializer {
  TextureInitializer() {
    sf::Image defaultImage(sf::Vector2u{16, 16}, sf::Color::White);
    (void)sharedProjTexture.loadFromImage(defaultImage);
  }
} projTextureInit;

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f direction) : sprite(sharedProjTexture) , damage(5), pos(pos){

    if(!loaded){
        (void)sharedProjTexture.loadFromFile("proj.png");
        sharedProjTexture.setSmooth(false);
        loaded = true;
    }

    float speed = 800.0f;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.0f) {
        velocity.x = (direction.x / length) * speed;
        velocity.y = (direction.y / length) * speed;
    } else {
        velocity = {0.0f, -speed};
    }

    sprite.setTexture(sharedProjTexture);
    sprite.setScale({1, -1});
    sprite.setPosition(pos);
}

Projectile::~Projectile() = default;

bool Projectile::checkOutOfBounds(sf::RenderWindow *window){
    sf::View currentView = window->getView();
    sf::FloatRect viewBounds(
        currentView.getCenter() - currentView.getSize() / 2.f,
        currentView.getSize()
    );
    
    sf::Vector2f projPos = sprite.getPosition();
    
    float margin = 100.0f; 
    if(projPos.x > viewBounds.position.x + viewBounds.size.x + margin || 
       projPos.x < viewBounds.position.x - margin ||
       projPos.y > viewBounds.position.y + viewBounds.size.y + margin || 
       projPos.y < viewBounds.position.y - margin)
    {
       return true;
    }
    return false;
}

void Projectile::move(float dt){
    pos.x += velocity.x * dt;
    pos.y += velocity.y * dt;
    sprite.setPosition(pos);
}

const sf::Sprite& Projectile::getSprite() const {
    return sprite;
}