#include "ScoringArchivo.h"

ScoringArchivo::ScoringArchivo() {}

// a este metodo tienen que pasarle un vector de 10 Scorings creado previamente y escribe sobre él.
void ScoringArchivo::leerPuntuaciones(Scoring puntuaciones[10]) {
    FILE* file = fopen("scoring.dat", "rb");

    // si no existe lo creo con scorings vacios y lo cierro.
    if (file == nullptr) {
        std::cout << "No se pudo abrir 'scoring.dat'. Aun no han guardado puntuaciones." << std::endl;
        return;
    }

    //fseek(file, 0, SEEK_SET);
    for (int i=0; i<10; i++) {
        if (fread(&puntuaciones[i], sizeof(Scoring), 1, file) != 1) break;
    }

    fclose(file);
}


bool ScoringArchivo::guardarNuevaPuntuacion(Scoring nuevaPuntuacion) {
    FILE* file = fopen("scoring.dat", "rb");
    Scoring vecAux[11];
    int cantidadRegistros = 0;

    if (file != nullptr) {
        while (cantidadRegistros < 10 && fread(&vecAux[cantidadRegistros], sizeof(Scoring), 1, file) == 1) {
            cantidadRegistros++;
        }
        fclose(file);
    }

    vecAux[cantidadRegistros] = nuevaPuntuacion; //pongo la nueva puntuacion al final.
    cantidadRegistros++;

    ordenarPuntuaciones(vecAux, cantidadRegistros);


    file = fopen("scoring.dat", "wb");
    if (file == nullptr) {
        std::cout << "No se pudo guardar la puntuacion." << std::endl;
        return false;
    }

    fwrite(vecAux, sizeof(Scoring), 10, file);

    fclose(file);
    return true;
}


void ScoringArchivo::ordenarPuntuaciones(Scoring puntuaciones[], int cantidadRegistros) {

    // Ordeno por seleccion.
    int posMin;
    Scoring aux;
    for (int i=0; i<cantidadRegistros-1 ; i++) {
        posMin = i;

        for (int j=i+1; j<cantidadRegistros; j++) {
            if (puntuaciones[j].getPuntuacionTotal() > puntuaciones[posMin].getPuntuacionTotal()) {
                posMin = j;
            }
        }

        aux = puntuaciones[i];
        puntuaciones[i] = puntuaciones[posMin];
        puntuaciones[posMin] = aux;
    }
}
