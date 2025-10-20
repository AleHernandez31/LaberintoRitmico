#pragma once
#include <SFML\Graphics.hpp>

class Player: public sf::Drawable {
    public:
        Player();
        void update();
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Sprite _sprite;
        sf::Texture _texture;
        float _velocity;
};
