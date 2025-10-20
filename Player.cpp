#include "Player.h"

Player::Player() {
    _texture.loadFromFile("assets/sprites/player.png");
    _sprite.setTexture(_texture);
    _velocity = 4;
}


void Player::update() {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _sprite.move(0, -_velocity);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _sprite.move(-_velocity, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _sprite.move(0, _velocity);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _sprite.move(_velocity, 0);
    }

    if (_sprite.getPosition().x < 0) {
        _sprite.setPosition(0, _sprite.getPosition().y);
    }

    if (_sprite.getPosition().y < 0) {
        _sprite.setPosition(_sprite.getPosition().x, 0);
    }

    if (_sprite.getPosition().x + _sprite.getGlobalBounds().width > 800) {
        _sprite.setPosition(800 - _sprite.getGlobalBounds().width, _sprite.getPosition().y);
    }

    if (_sprite.getPosition().y + _sprite.getGlobalBounds().height > 600) {
        _sprite.setPosition(_sprite.getPosition().x, 600 - _sprite.getGlobalBounds().height);
    }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}
