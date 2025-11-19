#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "PrototipoFuncionalidad.h"


PrototipoFuncionalidad::PrototipoFuncionalidad(float tamCelda, float tamCalle)
    : _celda(tamCelda), _calle(tamCalle) {

    srand(time(NULL));

    // Configuración inicial de texto de aciertos
    _fuente.loadFromFile("assets/fonts/comic.ttf");
    _texto.setFont(_fuente);
    _texto.setCharacterSize(30);
    _texto.setFillColor(sf::Color::Yellow);
    _texto.setOutlineColor(sf::Color::Black);
    _texto.setOutlineThickness(2.f);

    // circulo destino
    //float r = std::max(10.f, _celda * 0.18f);
    float r = 15.f;
    _circuloObjetivo.setRadius(r);
    _circuloObjetivo.setOrigin(r, r);
    _circuloObjetivo.setOutlineThickness(3);
    _circuloObjetivo.setOutlineColor(sf::Color::Black);

    _paletaNotas[0] = sf::Color(255,  64,  64); // rojo
    _paletaNotas[1] = sf::Color(255, 160,  64); // naranja
    _paletaNotas[2] = sf::Color(255, 220,  64); // amarillo
    _paletaNotas[3] = sf::Color( 64, 200,  64); // verde
    _paletaNotas[4] = sf::Color( 64, 160, 255); // celeste
    _paletaNotas[5] = sf::Color(160,  64, 255); // violeta
    _paletaNotas[6] = sf::Color(255,  64, 160); // rosa

}



// Arranca la ronda
void PrototipoFuncionalidad::iniciar(Player* player, const ConfigRitmo& cfg, Scoring* scoring) {
        _cfg = cfg;
        _relojJuego.restart(); // Pongo el reloj en 0
        _player = player; // Guardo la direccion de player en un puntero.
        (*_player).restaurarVida(); // Inicializo a vida en 100.
        const sf::Vector2i posPlayer = (*_player).getPosGrilla();
        _ultimaPos = posPlayer;
        _objetivoActivo = false;
        _indiceNota = 0;
        _scoring = scoring;

        int ahora = _relojJuego.getElapsedTime().asMilliseconds(); // Obtengo el tiempo transcurrido desde que reinicie el reloj.
        spawnearSiguiente(posPlayer, ahora);

        std::cout << "Inicio. objetivo en (" << _posObjetivo.x << "," << _posObjetivo.y << "). Objetivo tiempo total=" << _objetivoTiempoMs << "ms. Nota=" << _indiceNota << std::endl;
}



bool PrototipoFuncionalidad::actualizar(sf::RenderWindow& window, float dtSegundos) {
    int ahora = _relojJuego.getElapsedTime().asMilliseconds();
    const sf::Vector2i posPlayer = (*_player).getPosGrilla();

    // Si cambio de nodo, verifico como aterrizó.
    if (posPlayer != _ultimaPos) {
        enAterrizajeJugador(posPlayer, ahora);
        _ultimaPos = posPlayer;
    }

    // si se paso de la ventana "good", es bad
    if (_objetivoActivo && ahora > (_objetivoTiempoMs + _cfg.ventanaGoodMs)) {
        mostrarAcierto(aciertoGolpe::Bad, ahora - _objetivoTiempoMs);
        (*_player).reducirVida();
        spawnearSiguiente(posPlayer, ahora);
    }

    // Dibujuo el nuevo circulo (nota)
    if (_objetivoActivo) {
        sf::Vector2f posMundo = sf::Vector2f(_posObjetivo.x * _celda, _posObjetivo.y * _celda); // Convierto de celda a pixels
        _circuloObjetivo.setPosition(posMundo);
        _circuloObjetivo.setFillColor(_paletaNotas[_indiceNota]);
        window.draw(_circuloObjetivo);
    }

    dibujarStringAcierto(window);

    // Si se quedó sin vida devuelvo un false para, en el main, salir del modo jugando.
    if ((*_player).getVida() == 0) {
        return false;
    }

    return true;
}



// elijo el siguiente objetivo partiendo de una celda “base” y calculo el tiempo en que deberia atterrizar
void PrototipoFuncionalidad::spawnearSiguiente(const sf::Vector2i& posPlayer, int ahoraMs) {
    _posObjetivo = elegirVecino(posPlayer);
    _objetivoTiempoMs = ahoraMs + _cfg.cadenciaMs;
    int nuevaNota;
    do {
        nuevaNota = rand() % 7;
    } while (nuevaNota == _indiceNota); // Para que no se repitan dos notas seguidas
    _indiceNota = nuevaNota;
    _objetivoActivo = true;

    //std::cout << "Nuevo objetivo en (" << _posObjetivo.x << "," << _posObjetivo.y << "). Objetivo tiempo total=" << _objetivoTiempoMs << "ms. Nota=" << _indiceNota << std::endl;
}



// Validacion de aterrizaje
void PrototipoFuncionalidad::enAterrizajeJugador(const sf::Vector2i& posActual, int ahoraMs) {
    if (!_objetivoActivo) return;

   // if (posActual != _ultimaPos) {
   //     (*_scoring).sumarNotaAterrizada();
  //  }

    // si no cayo en el objetivo, es bad. igual seteo un nuevo objetivo
    if (posActual != _posObjetivo) {
        mostrarAcierto(aciertoGolpe::Bad, 0);
        (*_player).reducirVida();
        spawnearSiguiente(posActual, ahoraMs);
        return;
    }

    int delta = std::abs(ahoraMs - _objetivoTiempoMs);

    if (delta <= _cfg.ventanaPerfectMs) {
        mostrarAcierto(aciertoGolpe::Perfect, delta);
        (*_player).aumentarVida();
    } else if (delta <= _cfg.ventanaGoodMs) {
        mostrarAcierto(aciertoGolpe::Good, delta);
        (*_player).aumentarVida();
    } else {
        mostrarAcierto(aciertoGolpe::Bad, delta);
        (*_player).reducirVida();
    }

    // DDefino un nuevo objetivo
    spawnearSiguiente(posActual, ahoraMs);
}



// Elijo una celda vecina aletoriamente pero que no sea la celda anterior
sf::Vector2i PrototipoFuncionalidad::elegirVecino(const sf::Vector2i& celdaActual) {

    sf::Vector2i vecinos[4] = {
        {celdaActual.x + 1, celdaActual.y}, // derecha
        {celdaActual.x - 1, celdaActual.y}, // izquierda
        {celdaActual.x, celdaActual.y + 1}, // abajo
        {celdaActual.x, celdaActual.y - 1}  // arriba
    };

    // Elijo un vecino al azar, pero que no sea la celda anterior
    sf::Vector2i celdaElegida;
    do {
        int indiceAleatorio = rand() % 4;
        celdaElegida = vecinos[indiceAleatorio];
    } while (celdaElegida == _ultimaPos); // si es la celda de la que vengo, repito

    return celdaElegida;
}



// imprime el acierto en consola con el delta (+/- ms)
void PrototipoFuncionalidad::mostrarAcierto(aciertoGolpe aciertoGolpe, int deltaMs) {
    switch (aciertoGolpe) {
        case aciertoGolpe::Perfect:
            _ultimoAcierto = "PERFECT!";
            (*_scoring).sumarPerfect();
            //std::cout << "Perfect (" << strConSigno(deltaMs) << " ms)" << std::endl;
            break;
        case aciertoGolpe::Good:
            _ultimoAcierto = "GOOD";
            (*_scoring).sumarGood();
            //std::cout << "Good (" << strConSigno(deltaMs) << " ms)" << std::endl;
            break;
        case aciertoGolpe::Bad:
            _ultimoAcierto = "BAD";
            (*_scoring).sumarBad();
            //std::cout << "Bad (" << strConSigno(deltaMs) << " ms)" << std::endl;
            break;
    }

    (*_scoring).sumarMsAterrizaje(deltaMs);
    (*_scoring).sumarNotaAterrizada();
}



int PrototipoFuncionalidad::siguienteNota() {
    return _indiceNota;
}


// Util
std::string PrototipoFuncionalidad::strConSigno(int tiempo) {
    if (tiempo > 0) return "+" + std::to_string(tiempo);
    return std::to_string(tiempo);
}



void PrototipoFuncionalidad::dibujarStringAcierto(sf::RenderWindow& window) {
    sf::View view = window.getView();
    sf::Vector2f centro = view.getCenter();
    sf::Vector2f tamanio = view.getSize();
    _texto.setString(_ultimoAcierto);
    _texto.setPosition(centro.x, centro.y - (tamanio.y * 0.20f));
    _texto.setOrigin(_texto.getLocalBounds().width * 0.5f, _texto.getLocalBounds().height * 0.5f);
    window.draw(_texto);
}
