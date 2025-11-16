#pragma once
#include <iostream>

class Scoring {
    public:
        Scoring();
        void reiniciarScoring();
        void sumarPerfect();
        void sumarGood();
        void sumarBad();
        void sumarMsAterrizaje(float msNota);
        void sumarNotaAterrizada();
        void setNivelCompletado();
        void calcularPuntuacionTotal(int dificultadJugada);

        float getPuntuacionTotal();
        float getPromedioMsAterrizaje();
        bool getNivelCompletado();
        int getCantidadPerfects();
        int getCantidadGoods();
        int getCantidadBads();
        void toString();

    private:
        float _puntuacionTotal;
        float _promedioMsAterrizaje, _totalMsAterrizaje;
        int _cantidadPerfects, _cantidadGoods, _cantidadBads, _cantidadNotasAterrizadas;
        bool _nivelCompletado;

        void calcularPromedioMsAterrizaje(float totalMs, int cantidadNotasAterrizadas);
};
