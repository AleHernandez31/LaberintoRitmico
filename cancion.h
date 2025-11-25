#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct NotaCancion {
    float tiempoMs;       // Tiempo en MILLIsegundos en el que debe sonar / aterrizar la nota
    int   notaIndex;      // 0 a 6: C/Do, D/Re, E/Mi, F/Fa, G/Sol, A/La, B/Si
    sf::Vector2i gridPos; // Posicion en la grilla (fila, columna)
};

class Cancion {
public:
    Cancion(const std::string& pathCSV = "");

    // Carga o recarga la cancion desde CSV.
    // Devuelve true si pudo cargar al menos una nota.
    bool cargarDesdeCSV(const std::string& pathCSV);

    // Vuelve a la primera nota.
    void reiniciar();

    // Nota actual (puede devolver nullptr si no hay notas o se termino la cancion).
    const NotaCancion* getNotaActual() const;

    // Avanza a la siguiente nota. Devuelve false si ya no hay mas.
    bool avanzar();

    // Helpers
    bool vacia()   const;
    bool termino() const;

private:
    std::vector<NotaCancion> _notas;
    std::size_t _indiceActual;
    std::string _ultimoPath;

    int  notaStringToIndex(const std::string& s) const;
    void normalizarLinea(std::string& line) const;
};
