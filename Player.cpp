#include <cmath>
#include <iostream>
#include "Player.h"

Player::Player(float tamanioCelda, float velocidad, AudioControl &musicaJuego, sf::Vector2f posGrilla)
: _tamanioCelda(tamanioCelda), _velocidad(velocidad), _PosGrilla(posGrilla), _destino(0,0)
{
    _vida.setAudioControl(musicaJuego);

    if (_velocidad < 100) _velocidad = 100;   // Valido que la velocidad no sea nunca menor que 100.
    if (_velocidad > 2000) _velocidad = 2000; // Valido que la velocidad no sea nunca mayor que 2000.

    _texture.loadFromFile("assets/sprites/playerRojo.png");
    _sprite.setTexture(_texture);

    _sprite.setOrigin(_sprite.getLocalBounds().width * 0.5f,
                      _sprite.getLocalBounds().height * 0.75f);

    // Posición inicial (grid -> mundo)
    /*
        Como el tamaño de la celda esta en pixels multiplico ese valor por la cantidad de posiciones en donde este el jugador.
        Asi, si la posicion fuese (2,0) y la celda tiene un tamaño de 256px, el jugador se posicionaria en (512,0)
    */
    _sprite.setPosition(_PosGrilla.x * _tamanioCelda, _PosGrilla.y * _tamanioCelda); //

    // INICIALIZACIONES NUEVAS
    _PosGrillaAnt       = _PosGrilla; // la "celda anterior" arranca igual a la actual
    _destino            = _PosGrilla; // el destino inicial es donde ya está parado
    _esPrimerMovimiento = true;       // 🔹 primer movimiento libre

    // Inicialización del sistema de animación de dash
    _dirGrilla        = sf::Vector2i(0, 0);
    _dashAnimTimer    = 0.f;
    _dashAnimDuration = 0.12f;   // ~120ms de squash & stretch por dash
    _facingX          = 1.f;     // Arranca mirando a la derecha
    _estaDasheando    = false;
    _sprite.setScale(_facingX, 1.f);
}

// Proceso que tecla presionó el jugador.
void Player::manejadorEventos(const sf::Event& e) {
    if (e.type == sf::Event::KeyPressed) {
        if (_PosGrilla == _destino) { // Solo proceso el evento si el jugador esta en destino.
            switch (e.key.code) {
                case sf::Keyboard::W:
                    if (_esPrimerMovimiento || _PosGrilla + sf::Vector2i{0,-1} != _PosGrillaAnt) {
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 0, -1);

                        _dirGrilla       = sf::Vector2i(0, -1);
                        _dashAnimTimer   = 0.f;
                        _estaDasheando   = true;

                        _esPrimerMovimiento = false;
                    }
                    break;

                case sf::Keyboard::S:
                    if (_esPrimerMovimiento || _PosGrilla + sf::Vector2i{0,1} != _PosGrillaAnt) {
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 0,  1);

                        _dirGrilla       = sf::Vector2i(0, 1);
                        _dashAnimTimer   = 0.f;
                        _estaDasheando   = true;

                        _esPrimerMovimiento = false;
                    }
                    break;

                case sf::Keyboard::A:
                    if (_esPrimerMovimiento || _PosGrilla + sf::Vector2i{-1,0} != _PosGrillaAnt) {
                        _PosGrillaAnt = _PosGrilla;
                        setDestino(-1,  0);

                        _dirGrilla       = sf::Vector2i(-1, 0);
                        _dashAnimTimer   = 0.f;
                        _estaDasheando   = true;

                        _facingX = -1.f; // Flipeo imagen para que mire a la izquierda.
                        _esPrimerMovimiento = false;
                    }
                    break;

                case sf::Keyboard::D:
                    if (_esPrimerMovimiento || _PosGrilla + sf::Vector2i{1,0} != _PosGrillaAnt) {
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 1,  0);

                        _dirGrilla       = sf::Vector2i(1, 0);
                        _dashAnimTimer   = 0.f;
                        _estaDasheando   = true;

                        _facingX =  1.f; // Flipeo imagen para que mire a la derecha.
                        _esPrimerMovimiento = false;
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

// Seteo el destino al que se mueve el jugador.
void Player::setDestino(int destinoX, int destinoY) {
    _destino.x = _PosGrilla.x + destinoX;
    _destino.y = _PosGrilla.y + destinoY;
}

void Player::update(float deltaTiempo, int siguienteNota) {
    sf::Vector2f destino(_destino.x * _tamanioCelda, _destino.y * _tamanioCelda);
    sf::Vector2f posicion = _sprite.getPosition();
    sf::Vector2f diferencia = destino - posicion;

    float distancia = std::sqrt(diferencia.x * diferencia.x + diferencia.y * diferencia.y);

    // 🔹 Evitar division por cero
    if (distancia == 0.f) {
        // Ya estoy en el destino, no muevo nada.
    } else {
        sf::Vector2f direccion(diferencia.x / distancia, diferencia.y / distancia);
        float pasos = _velocidad * deltaTiempo;

        if (pasos >= distancia) {
            _sprite.setPosition(destino);
            _PosGrilla = _destino;

            // Al aterrizar, termina el dash
            _estaDasheando = false;
            _dashAnimTimer = 0.f;
        } else {
            _sprite.move(direccion * pasos);
        }
    }

    // Animacion de squash y stretch del dash
    float scaleX = 1.f;
    float scaleY = 1.f;

    if (_estaDasheando) {
        _dashAnimTimer += deltaTiempo;

        // t normalizado 0..1 dentro de la duración del dash
        float t = _dashAnimTimer / _dashAnimDuration;
        if (t > 1.f) t = 1.f;

        // Curva tipo "impulso": sube a 1 y vuelve a 0
        float fase;
        if (t < 0.5f) {
            fase = t / 0.5f;            // 0 -> 1 en la primera mitad
        } else {
            fase = 1.f - (t - 0.5f) / 0.5f; // 1 -> 0 en la segunda mitad
        }

        float stretch = 1.f + 0.25f * fase; // se estira
        float squash  = 1.f - 0.20f * fase; // se aplasta

        if (_dirGrilla.x != 0) {
            // Movimiento horizontal: estiro en X y aplasto en Y
            scaleX = stretch;
            scaleY = squash;
        }
        else if (_dirGrilla.y != 0) {
            // Movimiento vertical: estiro en Y y aplasto en X
            scaleX = squash;
            scaleY = stretch;
        }

        if (_dashAnimTimer >= _dashAnimDuration) {
            _estaDasheando = false;
        }
    }

    // Aplicamos el flip horizontal + squash/stretch
    _sprite.setScale(_facingX * scaleX, scaleY);

    // Cambio color de la guitarra de acuerdo a la nota siguiente
    switch (siguienteNota) {
        case 0:
            _texture.loadFromFile("assets/sprites/playerRojo.png");
            _sprite.setTexture(_texture);
            break;

        case 1:
            _texture.loadFromFile("assets/sprites/playerNaranja.png");
            _sprite.setTexture(_texture);
            break;

        case 2:
            _texture.loadFromFile("assets/sprites/playerAmarillo.png");
            _sprite.setTexture(_texture);
            break;

        case 3:
            _texture.loadFromFile("assets/sprites/playerVerde.png");
            _sprite.setTexture(_texture);
            break;

        case 4:
            _texture.loadFromFile("assets/sprites/playerCeleste.png");
            _sprite.setTexture(_texture);
            break;

        case 5:
            _texture.loadFromFile("assets/sprites/playerVioleta.png");
            _sprite.setTexture(_texture);
            break;

        case 6:
            _texture.loadFromFile("assets/sprites/playerRosa.png");
            _sprite.setTexture(_texture);
            break;

        default:
            std::cout << "Error en carga de sprite Player. Codigo recibido invalido.";
            break;
    }
}

void Player::reducirVida() {
    _vida.reducirVida();
}

void Player::aumentarVida() {
    _vida.aumentarVida();
}

void Player::restaurarVida() {
    _vida.restaurarVida();
}

float Player::getVida() {
    return _vida.getVida();
}

sf::Vector2i Player::getPosGrilla() {
    return _PosGrilla;
}

sf::Vector2f Player::getPosMundo() {
    return _sprite.getPosition();
}

void Player::reiniciarMovimiento() {
    _PosGrillaAnt       = _PosGrilla;
    _destino            = _PosGrilla;
    _esPrimerMovimiento = true;

    _dirGrilla        = sf::Vector2i(0, 0);
    _dashAnimTimer    = 0.f;
    _estaDasheando    = false;
    _facingX          = 1.f;
    _sprite.setScale(_facingX, 1.f);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}
