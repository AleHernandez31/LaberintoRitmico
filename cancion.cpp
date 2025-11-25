#include "Cancion.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

Cancion::Cancion(const std::string& pathCSV)
    : _indiceActual(0), _ultimoPath(pathCSV) {

    if (!pathCSV.empty()) {
        cargarDesdeCSV(pathCSV);
    }
}

bool Cancion::cargarDesdeCSV(const std::string& pathCSV) {
    _notas.clear();
    _indiceActual = 0;
    _ultimoPath = pathCSV;

    std::ifstream file(pathCSV);
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo de la cancion: " << pathCSV << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        normalizarLinea(line);
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string campoTime, campoNota, campoFila, campoCol;

        if (!std::getline(ss, campoTime, ',')) continue;
        if (!std::getline(ss, campoNota, ',')) continue;
        if (!std::getline(ss, campoFila, ',')) continue;
        if (!std::getline(ss, campoCol, ',')) continue;

        try {
            float t = std::stof(campoTime);
            int idx = notaStringToIndex(campoNota);
            int fila = std::stoi(campoFila);
            int col  = std::stoi(campoCol);

            NotaCancion nota;
            nota.tiempoMs = t * 1000.0f;         // Convertimos a ms para matchear con el reloj del juego
            nota.notaIndex = idx;
            nota.gridPos = sf::Vector2i(fila, col);

            _notas.push_back(nota);
        }
        catch (...) {

            continue;
        }
    }

    // Aseguramos que estén ordenadas por tiempo por las dudas
    std::sort(_notas.begin(), _notas.end(), [](const NotaCancion& a, const NotaCancion& b){
        return a.tiempoMs < b.tiempoMs;
    });

    if (_notas.empty()) {
        std::cout << "El archivo de cancion '" << pathCSV << "' no contiene notas validas." << std::endl;
        return false;
    }

    return true;
}

void Cancion::reiniciar() {
    _indiceActual = 0;
}

const NotaCancion* Cancion::getNotaActual() const {
    if (_indiceActual >= _notas.size()) return nullptr;
    return &_notas[_indiceActual];
}

bool Cancion::avanzar() {
    if (_indiceActual + 1 < _notas.size()) {
        _indiceActual++;
        return true;
    }
    _indiceActual = _notas.size();
    return false;
}

bool Cancion::vacia() const {
    return _notas.empty();
}

bool Cancion::termino() const {
    return _notas.empty() || _indiceActual >= _notas.size();
}

// Helpers:

void Cancion::normalizarLinea(std::string& line) const {

    std::replace(line.begin(), line.end(), ';', ',');
    // Trim básico
    while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back()))) line.pop_back();
    while (!line.empty() && std::isspace(static_cast<unsigned char>(line.front()))) line.erase(line.begin());
}

int Cancion::notaStringToIndex(const std::string& s) const {

    //  Letras: C D E F G A B
    //  Nombres: DO RE MI FA SOL LA SI
    //  Indices numericos 0..6 como texto
    std::string n;
    n.reserve(s.size());
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c)))
            n.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }

    if (n.empty()) return 0;

    if (n.size() == 1) {
        switch (n[0]) {
            case 'C': return 0;
            case 'D': return 1;
            case 'E': return 2;
            case 'F': return 3;
            case 'G': return 4;
            case 'A': return 5;
            case 'B': return 6;
            default: break;
        }
    }

    if (n == "DO")   return 0;
    if (n == "RE")   return 1;
    if (n == "MI")   return 2;
    if (n == "FA")   return 3;
    if (n == "SOL")  return 4;
    if (n == "LA")   return 5;
    if (n == "SI")   return 6;

    try {
        int v = std::stoi(n);
        if (v >= 0 && v <= 6) return v;
    } catch (...) {}

    //Do es el default
    return 0;
}
