#include <cmath>
#include <iostream>
#include "Player.h"

Player::Player(float tamanioCelda, float velocidad, sf::Vector2f posGrilla)
: _tamanioCelda(tamanioCelda), _velocidad(velocidad), _PosGrilla(posGrilla), _destino(0,0)
{
    if (_velocidad < 100) _velocidad = 100; // Valido que la velocidad no sea nunca menor que 100.
    if (_velocidad > 2000) _velocidad = 2000; // Valido que la velocidad no sea nunca mayor que 2000.

    _texture.loadFromFile("assets/sprites/playerRojo.png");
    _sprite.setTexture(_texture);

    _sprite.setOrigin(_sprite.getLocalBounds().width * 0.5f, _sprite.getLocalBounds().height *0.75f);

    // Posición inicial (grid -> mundo)
    /*
        Como el tamaño de la celda esta en pixels multiplico ese valor por la cantidad de posiciones en donde este el jugador.
        Asi, si la posicion fuese (2,0) y la celda tiene un tamaño de 256px, el jugador se posicionaria en (512,0)
    */
    _sprite.setPosition(_PosGrilla.x * _tamanioCelda, _PosGrilla.y * _tamanioCelda); //
}


// Proceso que tecla presionó el jugador.
void Player::manejadorEventos(const sf::Event& e) {
    if (e.type == sf::Event::KeyPressed) {
        if (_PosGrilla == _destino) { // Solo proceso el evento si el jugador esta en destino. Prevengo procesar eventos mientras se esta moviendo.
            switch (e.key.code) {
                case sf::Keyboard::W:
                    if (_PosGrilla + sf::Vector2i{0,-1} != _PosGrillaAnt) { // Solo proceso el evento si el jugador no esta volviendo hacia atras
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 0, -1);
                    }
                    break;

                case sf::Keyboard::S:
                    if (_PosGrilla + sf::Vector2i{0,1} != _PosGrillaAnt) { // Solo proceso el evento si el jugador no esta volviendo hacia atras
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 0,  1);
                    }
                    break;

                case sf::Keyboard::A:
                    if (_PosGrilla + sf::Vector2i{-1,0} != _PosGrillaAnt) { // Solo proceso el evento si el jugador no esta volviendo hacia atras
                        _PosGrillaAnt = _PosGrilla;
                        setDestino(-1,  0);
                        _sprite.setScale(-1.f, 1.f); // Flipeo imagen para que mire a la izquierda.
                    }
                    break;

                case sf::Keyboard::D:
                    if (_PosGrilla + sf::Vector2i{1,0} != _PosGrillaAnt) { // Solo proceso el evento si el jugador no esta volviendo hacia atras
                        _PosGrillaAnt = _PosGrilla;
                        setDestino( 1,  0);
                        _sprite.setScale( 1.f, 1.f); // Flipeo imagen para que mire a la derecha.
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
    sf::Vector2f destino(_destino.x * _tamanioCelda, _destino.y * _tamanioCelda); // Defino el destino.
    sf::Vector2f posicion = _sprite.getPosition(); // Defino la posicion.
    sf::Vector2f diferencia = destino - posicion; // Defino la diferencia entre ambas coordenadas.

    // Aplico Pitagoras para tener la distancia total hasta el destino
    float distancia = std::sqrt(diferencia.x * diferencia.x + diferencia.y * diferencia.y);

    // Calculo cuanto se tiene que mover y lo muevo
    sf::Vector2f direccion(diferencia.x / distancia, diferencia.y / distancia);
    float pasos = _velocidad * deltaTiempo;

    // Si se tiene que mover mas de lo que falta para llegar lo posiciono directamente en el destino.
    if (pasos >= distancia) {
        _sprite.setPosition(destino);
        _PosGrilla = _destino;
    } else {
        _sprite.move(direccion * pasos); // Lo muevo la cantidad de pasos que corresponda en la direccion que corresponda.
    }

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

    /*
        _paletaNotas[0] = sf::Color(255,  64,  64); // rojo
        _paletaNotas[1] = sf::Color(255, 160,  64); // naranja
        _paletaNotas[2] = sf::Color(255, 220,  64); // amarillo
        _paletaNotas[3] = sf::Color( 64, 200,  64); // verde
        _paletaNotas[4] = sf::Color( 64, 160, 255); // celeste
        _paletaNotas[5] = sf::Color(160,  64, 255); // violeta
        _paletaNotas[6] = sf::Color(255,  64, 160); // rosa
    */

}



sf::Vector2i Player::getPosGrilla() {
    return _PosGrilla;
}



sf::Vector2f Player::getPosMundo() {
    return _sprite.getPosition();
}



void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}
