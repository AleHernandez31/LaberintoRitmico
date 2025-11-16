#pragma once
#include "Scoring.h"

// cree esta clase para que se encargue de persistir las puntuaciones (objeto Scroring) en un archivo.
class ScoringArchivo {
    public:
        ScoringArchivo();
        void leerPuntuaciones(Scoring puntuaciones[10]);
        bool guardarNuevaPuntuacion(Scoring nuevaPuntuacion);

    private:
        void ordenarPuntuaciones(Scoring puntuaciones[], int cantidadRegistros);

};
