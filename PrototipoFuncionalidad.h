#pragma once
#include <SFML/Graphics.hpp>

// config basica para el prototipo, despues vemos de hacerlo con un Archivo
struct ConfigRitmo {
    int ventanaPerfectMs = 50;
    int ventanaGoodMs    = 120;
    int cadenciaMs       = 500;
};


enum class aciertoGolpe {
    Perfect,
    Good,
    Bad
};

// clase prototipo para probar la mecanica. dibuja solo el circulo objetivo. el feedback de acierto se muestra por consola por ahora
class PrototipoFuncionalidad {
    public:
        PrototipoFuncionalidad(float tamCelda, float tamCalle);
        void iniciar(const sf::Vector2i& posPlayer, const ConfigRitmo& cfg);
        void actualizar(const sf::Vector2i& posPlayer, sf::RenderWindow& ventana, float dtSegundos);
        sf::Vector2i objetivoActual();
        int tiempoObjetivoMs();
        bool hayObjetivo();

    private:
        void spawnearSiguiente(const sf::Vector2i& desdeGrilla, int ahoraMs);
        void enAterrizajeJugador(const sf::Vector2i& posCaida, int ahoraMs);
        sf::Vector2i elegirVecino(const sf::Vector2i& g);
        void mostrarAcierto(aciertoGolpe aciertoGolpe, int deltaFirmadoMs);
        int tiempoMs();
        std::string strConSigno(int x);
        sf::Vector2f grillaACentroMundo(const sf::Vector2i& g);

        float _celda = 0.f;
        float _calle = 0.f;
        ConfigRitmo _cfg{};
        sf::Clock _relojJuego{};
        bool _objetivoActivo = false;
        sf::Vector2i _objetivoGrilla{0, 0};
        int _objetivoTiempoMs = 0;
        int _indiceNota = 0;
        sf::Vector2i _ultimaPos{INT32_MIN, INT32_MIN};
        sf::CircleShape _circuloObjetivo;
        std::array<sf::Color, 7> _paletaNotas{};
};
