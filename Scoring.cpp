#include "Scoring.h"

Scoring::Scoring():
    _puntuacionTotal(0),
    _totalMsAterrizaje(0),
    _cantidadPerfects(0),
    _cantidadGoods(0),
    _cantidadBads(0),
    _cantidadNotasAterrizadas(0),
    _nivelCompletado(false) {

}


void Scoring::reiniciarScoring() {
    _puntuacionTotal = 0;
    _totalMsAterrizaje = 0;
    _cantidadPerfects = 0;
    _cantidadGoods = 0;
    _cantidadBads = 0;
    _cantidadNotasAterrizadas = 0;
    _nivelCompletado = false;
}


void Scoring::sumarPerfect() {
    _cantidadPerfects++;
}


void Scoring::sumarGood() {
    _cantidadGoods++;
}


void Scoring::sumarBad() {
    _cantidadBads++;
}


void Scoring::sumarMsAterrizaje(float msNota) {
    _totalMsAterrizaje += msNota;
}


void Scoring::sumarNotaAterrizada() {
    _cantidadNotasAterrizadas++;
}


void Scoring::setNivelCompletado() {
    _nivelCompletado = true;
}



float Scoring::getPuntuacionTotal() {
    return _puntuacionTotal;
}


float Scoring::getPromedioMsAterrizaje() {
    return _promedioMsAterrizaje;
}


bool Scoring::getNivelCompletado() {
    return _nivelCompletado;
}


int Scoring::getCantidadPerfects() {
    return _cantidadPerfects;
}


int Scoring::getCantidadGoods() {
    return _cantidadGoods;
}


int Scoring::getCantidadBads() {
    return _cantidadBads;
}



void Scoring::calcularPromedioMsAterrizaje(float totalMs, int cantidadNotasAterrizadas) {
    if (cantidadNotasAterrizadas > 0) {
        _promedioMsAterrizaje = totalMs / cantidadNotasAterrizadas;
    } else std::cout << "Error al calcular promedio de ms de aterrizaje." << std::endl;
}


void Scoring::calcularPuntuacionTotal(int dificultadJugada) {
    // Puntos de cada nota
    const float PTS_PERFECT = 100.f;
    const float PTS_GOOD = 70.f;
    const float PTS_BAD = 0;

    // Multiplicadores por dificultad
    const float MULTIPLICADOR_FACIL   = 1.f;
    const float MULTIPLICADOR_NORMAL  = 1.25f;
    const float MULTIPLICADOR_DIFICIL = 1.50f;

    // Calculo puntaje baje
    float puntajeBase = (_cantidadPerfects * PTS_PERFECT) + (_cantidadGoods * PTS_GOOD) + (_cantidadBads * PTS_BAD);

    // Si no se aterrizo en ninguna nota le pongo 0 de puntaje
    if (_cantidadNotasAterrizadas == 0) {
        _puntuacionTotal = 0.f;
        return;
    }

    // Saco el promedio total de ms en los aterrizajes
    calcularPromedioMsAterrizaje(_totalMsAterrizaje, _cantidadNotasAterrizadas);

    // Penalizo por tiempo de aterrizaje promedio.
    // Un tiempo promedio de 10ms penaliza 1% (baja el multiplicador a 0.99).
    // Un tiempo promedio de 100ms penaliza 10% (baja el multiplicador a 0.9).
    // Un tiempo promedio de 300ms penaliza 30% (baja el multiplicador a 0.7).
    // Un tiempo promedio de 1000ms penaliza 100% (baja el multiplicador a 0, osea 0 puntos).
    float penalizacion = std::max(0.f, 1.f - (_promedioMsAterrizaje / 1000.f));
    _puntuacionTotal = puntajeBase * penalizacion;

    // Si no hubieron Bads y se completó la canción otorgo un +10% extra como bonificación.
    if (_nivelCompletado == true && _cantidadBads == 0 && _cantidadNotasAterrizadas > 0) {
        _puntuacionTotal *=  1.1f;
    }

    // Modifico el puntaje de acuerdo al nivel de dificultad jugado.
    switch(dificultadJugada) {
        case 1:
            _puntuacionTotal *= MULTIPLICADOR_FACIL;
            break;

        case 2:
            _puntuacionTotal *= MULTIPLICADOR_NORMAL;
            break;

        case 3:
            _puntuacionTotal *= MULTIPLICADOR_DIFICIL;
            break;

        default:
            break;
    }
}


void Scoring::toString() {
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Puntuacion total: " << this->getPuntuacionTotal() << std::endl;
    std::cout << "Tiempo promedio de aterrizaje: " << this->getPromedioMsAterrizaje() << "ms" << std::endl;
    std::cout << "Perfects: " << this->getCantidadPerfects() << std::endl;
    std::cout << "Goods: " << this->getCantidadGoods() << std::endl;
    std::cout << "Bads: " << this->getCantidadBads() << std::endl;
    std::cout << "----------------------------------" << std::endl;
}
