#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "AudioControl.h"
#include "Vida.h"

class Player : public sf::Drawable {
public:
    Player(float tamanioCelda, float velocidad, AudioControl &musicaJuego, sf::Vector2f posGrilla);
    void manejadorEventos(const sf::Event& e); // Tomo el evento desde main y muevo al jugador en consecuencia.
    void update(float deltaTiempo, int siguienteNota); // Actualizo la posición del jugador.
    sf::Vector2i getPosGrilla();
    sf::Vector2f getPosMundo();
    void reducirVida();
    void aumentarVida();
    void restaurarVida();
    float getVida();
    void reiniciarMovimiento();

private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    float _tamanioCelda;
    float _velocidad;
    Vida _vida;
    sf::Vector2i _PosGrilla;     // Posición actual en la grilla
    sf::Vector2i _PosGrillaAnt;  // Posicion anterior en la grilla
    sf::Vector2i _destino;       // Destino en la grilla

    bool _esPrimerMovimiento = true; //aaaa

    // --- Juice de movimiento: animación de dash (squash & stretch) ---
    sf::Vector2i _dirGrilla;        // Dirección del último movimiento en la grilla
    float _dashAnimTimer;           // Tiempo transcurrido desde que empezó el dash
    float _dashAnimDuration;        // Duración total de la animación de squash y stretch
    float _facingX;                 // 1 = mirando a la derecha, -1 = mirando a la izquierda
    bool  _estaDasheando;           // true mientras está en pleno dash

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setDestino(int destinoX, int destinoY); // Seteo el destino al que se mueve el player. La uso desde "manejadorEventos".
};
