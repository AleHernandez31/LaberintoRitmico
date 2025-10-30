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
        void actualizar(const sf::Vector2i& posPlayer, sf::RenderWindow& window, float dtSegundos);
        int siguienteNota();

    private:
        void spawnearSiguiente(const sf::Vector2i& desdeGrilla, int ahoraMs);
        void enAterrizajeJugador(const sf::Vector2i& posCaida, int ahoraMs);
        sf::Vector2i elegirVecino(const sf::Vector2i& g);
        void mostrarAcierto(aciertoGolpe aciertoGolpe, int deltaFirmadoMs);
        std::string strConSigno(int x);
        void dibujarStringAcierto(sf::RenderWindow& window);

        float _celda = 0.f;
        float _calle = 0.f;
        int _objetivoTiempoMs = 0;
        int _indiceNota = 0;
        bool _objetivoActivo = false;
        ConfigRitmo _cfg{};
        std::string _ultimoAcierto;
        sf::Clock _relojJuego{};
        sf::Vector2i _posObjetivo{0, 0};
        sf::Vector2i _ultimaPos;
        sf::CircleShape _circuloObjetivo;
        sf::Color _paletaNotas[7];
        sf::Font _fuente;
        sf::Text _textoAcierto;
};
