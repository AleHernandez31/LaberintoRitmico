#include "Player.h"

Player::Player() {
    _velocity = {4, 4};
    _texture.loadFromFile("assets/sprites/player.png");
    _sprite.setTexture(_texture);
    // Seteo el origen del sprite en la mitad en el eje X y en la parte inferior en Y, para luego hacer un flip.
    _sprite.setOrigin(_sprite.getGlobalBounds().width/2, _sprite.getGlobalBounds().height);
}


void Player::update() {

    _velocity = {0, 0};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _velocity.y = -4;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _velocity.x = -4;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _velocity.y = 4;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _velocity.x = 4;
    }

    _sprite.move(_velocity);

    // Si muevo a la izquierda flipeo la imagen "hacia atras".
    if (_velocity.x < 0) {
        _sprite.setScale(-1, 1);
    }

    // Si muevo a la izquierda flipeo la imagen "hacia adelante".
    if (_velocity.x > 0) {
        _sprite.setScale(1, 1);
    }

    // Valido si me voy de la pantalla por la izquierda y reposiciono.
    if (_sprite.getGlobalBounds().left < 0) {
        _sprite.setPosition(_sprite.getOrigin().x, _sprite.getPosition().y);
    }

    // Valido si me voy de la pantalla por arriba y reposiciono.
    if (_sprite.getGlobalBounds().top < 0) {
        _sprite.setPosition(_sprite.getPosition().x, _sprite.getOrigin().y);
    }

    // Valido si me voy de la pantalla por la derecha y reposiciono.
    if (_sprite.getGlobalBounds().left + _sprite.getGlobalBounds().width > 800) {
        _sprite.setPosition(800 - _sprite.getOrigin().x, _sprite.getPosition().y);;
    }

    // Valido si me voy de la pantalla por abajo y reposiciono.
    if (_sprite.getGlobalBounds().top + _sprite.getGlobalBounds().height > 600) {
        _sprite.setPosition(_sprite.getPosition().x, 600);
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}
