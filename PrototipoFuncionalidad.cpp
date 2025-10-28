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

    // circulo destino
    float r = std::max(10.f, _celda * 0.18f);
    _circuloObjetivo.setRadius(r);
    _circuloObjetivo.setOrigin(r, r);
    _circuloObjetivo.setOutlineThickness(std::max(2.f, r * 0.18f));
    _circuloObjetivo.setOutlineColor(sf::Color::Black);

    _paletaNotas = {
        sf::Color(255,  64,  64), // rojo
        sf::Color(255, 160,  64), // naranja
        sf::Color(255, 220,  64), // amarillo
        sf::Color( 64, 200,  64), // verde
        sf::Color( 64, 160, 255), // celeste
        sf::Color(160,  64, 255), // violeta
        sf::Color(255,  64, 160)  // magenta
    };
}



// Arranca la ronda
void PrototipoFuncionalidad::iniciar(const sf::Vector2i& posPlayer, const ConfigRitmo& cfg) {
        _cfg = cfg;
        _relojJuego.restart(); // Reinicio el reloj
        _ultimaPos = posPlayer;
        _objetivoActivo = false;
        _indiceNota = 0;

        int ahora = tiempoMs(); // Obtengo el tiempo transcurrido desde que reinicie el reloj.
        spawnearSiguiente(posPlayer, ahora);

        std::cout << "Inicio. objetivo en (" << _objetivoGrilla.x << "," << _objetivoGrilla.y << ") a tiempo=" << _objetivoTiempoMs << "ms, nota=" << _indiceNota << std::endl;
}



void PrototipoFuncionalidad::actualizar(const sf::Vector2i& posPlayer, sf::RenderWindow& ventana, float dtSegundos) {
    int ahora = tiempoMs();

    // detectar aterrizaje: cambio de celda entre frames
    if (posPlayer != _ultimaPos) {
        enAterrizajeJugador(posPlayer, ahora);
        _ultimaPos = posPlayer;
    }

    // si se paso de la ventana "good" sin caer, es tarde (bad) y generamos otro
    if (_objetivoActivo && ahora > (_objetivoTiempoMs + _cfg.ventanaGoodMs)) {
        mostrarAcierto(aciertoGolpe::Bad, ahora - _objetivoTiempoMs);
        spawnearSiguiente(posPlayer, ahora);
    }

    // dibujar el circulo del objetivo (color por “nota”)
    if (_objetivoActivo) {
        sf::Vector2f posMundo = grillaACentroMundo(_objetivoGrilla);
        _circuloObjetivo.setPosition(posMundo);
        _circuloObjetivo.setFillColor(_paletaNotas[_indiceNota]);
        ventana.draw(_circuloObjetivo);
    }
}



// helpers por si queres consultar algo desde afuera (debug)
sf::Vector2i PrototipoFuncionalidad::objetivoActual() {
    return _objetivoGrilla;
}



int PrototipoFuncionalidad::tiempoObjetivoMs(){
    return _objetivoTiempoMs;
}



bool PrototipoFuncionalidad::hayObjetivo() {
    return _objetivoActivo;
}



// elijo el siguiente objetivo partiendo de una celda “base” y calculo el tiempo en que deberia hacerlo
void PrototipoFuncionalidad::spawnearSiguiente(const sf::Vector2i& desdeGrilla, int ahoraMs) {
    _objetivoGrilla = elegirVecino(desdeGrilla);
    _objetivoTiempoMs = ahoraMs + _cfg.cadenciaMs;
    int nuevaNota;
    do {
        nuevaNota = rand() % 7;
    } while (nuevaNota == _indiceNota);
    _indiceNota = nuevaNota;
    _objetivoActivo = true;

    std::cout << "Nuevo objetivo en (" << _objetivoGrilla.x << "," << _objetivoGrilla.y << ") para tiempo=" << _objetivoTiempoMs << "ms, nota=" << _indiceNota << std::endl;
}



// cuando el jugador cae en alguna celda
void PrototipoFuncionalidad::enAterrizajeJugador(const sf::Vector2i& posCaida, int ahoraMs) {
    if (!_objetivoActivo) return;

    // si no cayo en el objetivo, es bad. igual avanzamos la secuencia
    if (posCaida != _objetivoGrilla) {
        mostrarAcierto(aciertoGolpe::Bad, 0);
        spawnearSiguiente(posCaida, ahoraMs);
        return;
    }

    const int delta = std::abs(ahoraMs - _objetivoTiempoMs);

    if (delta <= _cfg.ventanaPerfectMs) {
        mostrarAcierto(aciertoGolpe::Perfect, ahoraMs - _objetivoTiempoMs);
    } else if (delta <= _cfg.ventanaGoodMs) {
        mostrarAcierto(aciertoGolpe::Good, ahoraMs - _objetivoTiempoMs);
    } else {
        mostrarAcierto(aciertoGolpe::Bad, ahoraMs - _objetivoTiempoMs);
    }

    // seguir con la siguiente meta desde donde estas
    spawnearSiguiente(posCaida, ahoraMs);
}



// elige un vecino 4-dir y evita volver a la celda anterior (anti retroceso)
sf::Vector2i PrototipoFuncionalidad::elegirVecino(const sf::Vector2i& celdaActual) {
    // Celdas vecinas posibles (derecha, izquierda, abajo, arriba)
    sf::Vector2i vecinos[4] = {
        {celdaActual.x + 1, celdaActual.y}, // derecha
        {celdaActual.x - 1, celdaActual.y}, // izquierda
        {celdaActual.x, celdaActual.y + 1}, // abajo
        {celdaActual.x, celdaActual.y - 1}  // arriba
    };

    // Elijo un vecino al azar, pero no permito volver a la celda anterior
    sf::Vector2i celdaElegida;
    do {
        int indiceAleatorio = rand() % 4;           // valor entre 0 y 3
        celdaElegida = vecinos[indiceAleatorio];
    } while (celdaElegida == _ultimaPos);        // si es la celda de la que vengo, repito

    return celdaElegida;
}



// imprime el acierto en consola con el delta firmado (+/- ms)
void PrototipoFuncionalidad::mostrarAcierto(aciertoGolpe aciertoGolpe, int deltaFirmadoMs) {
    switch (aciertoGolpe) {
        case aciertoGolpe::Perfect:
            std::cout << "Perfect (" << strConSigno(deltaFirmadoMs) << " ms)" << std::endl;
            break;
        case aciertoGolpe::Good:
            std::cout << "Good (" << strConSigno(deltaFirmadoMs) << " ms)" << std::endl;
            break;
        case aciertoGolpe::Bad:
            // si queres marcar “late” podrias chequear delta>0, pero bueh
            std::cout << "Bad (" << strConSigno(deltaFirmadoMs) << " ms)" << std::endl;
            break;
    }
}



// Util
int PrototipoFuncionalidad::tiempoMs() {
    return _relojJuego.getElapsedTime().asMilliseconds(); // Obtengo el tiempo transcurrido desde que el ultimo reinicio del reloj.
}



std::string PrototipoFuncionalidad::strConSigno(int x) {
    if (x > 0) return "+" + std::to_string(x);
    return std::to_string(x);
}



// pasar de celda a centro en mundo (coincide con como ustedes dibujan la grilla)
sf::Vector2f PrototipoFuncionalidad::grillaACentroMundo(const sf::Vector2i& g) {
    return sf::Vector2f(g.x * _celda, g.y * _celda);
}
