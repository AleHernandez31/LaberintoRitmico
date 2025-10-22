#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Drawable {
public:
    Player(float tamanioCelda, float velocidad, sf::Vector2f posGrilla);
    void manejadorEventos(const sf::Event& e); // Tomo el evento desde main y muevo al jugador en consecuencia.
    void update(float deltaTiempo); // Actualizo la posición del jugador.
    sf::Vector2i getPosGrilla();
    sf::Vector2f getPosMundo();


private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    float _tamanioCelda;
    float _velocidad;
    sf::Vector2i _PosGrilla; // Posición actual en la grilla
    sf::Vector2i _destino; // Destino en la grilla

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setDestino(int destinoX, int destinoY); // Seteo el destino al que se mueve el player. La uso desde "manejadorEventos".
};
