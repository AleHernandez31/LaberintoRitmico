#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> //para los sonidos de hit y fail
#include <vector>
#include <string>
#include "Player.h"
#include "Scoring.h"
#include "Cancion.h"
#include "ParticleSystemNodo.h"


// Config básica para el sistema de ritmo
struct ConfigRitmo {
    // Ventanas de tiempo en milisegundos (EARLY y LATE)
    int ventanaPerfectMsEarly = -60;   // llega hasta 60 ms antes -> PERFECT
    int ventanaPerfectMsLate  = +40;   // hasta 40 ms después

    int ventanaGoodMsEarly    = -130;  // llega 130 ms antes -> GOOD
    int ventanaGoodMsLate     = +110;  // llega 110 ms después

    // Offset global: adelantar/atrasar el objetivo respecto al CSV
    // (negativo = el juego espera que golpees un poco antes del tiempo real)
    int offsetMs              = -60;
};

enum class aciertoGolpe {
    Perfect,
    Good,
    Bad
};

// Clase final para manejar la mecánica de nodos sincronizada con la canción
class GestorNodos {
public:
    GestorNodos(float tamCelda, float tamCalle);

    // Inicia el sistema de nodos para una partida
    void iniciar(Player* player, Cancion* cancion, const ConfigRitmo& cfg, Scoring* scoring);


    // Devuelve false si se termina la partida (vida en 0 o canción terminada).
    bool actualizar(sf::RenderWindow& window, float dtSegundos);

    // Devuelve el índice de la nota actual (0..6) para que el Player cambie el sprite.
    int siguienteNota() const;

private:
    // Avanza a la siguiente entrada del CSV y genera un nuevo objetivo + trampas
    void avanzarANotaSiguiente();

    // Llamada cuando el jugador aterriza en una nueva celda
    void enAterrizajeJugador(const sf::Vector2i& posActual, int ahoraMs);

    void mostrarAcierto(aciertoGolpe acierto, int deltaMs);
    std::string strConSigno(int x) const;
    void dibujarStringAcierto(sf::RenderWindow& window);

    // Dispara un burst de partículas según el tipo de acierto
    void dispararParticulas(aciertoGolpe acierto);

    // Genera el nodo correcto y los nodos trampa alrededor del jugador
    void generarObjetivoYTrampas(const sf::Vector2i& celdaBase);

    // Actualiza radio / alpha del halo de tempo según el tiempo actual
    // El halo se cierra de afuera hacia adentro hasta tocar el nodo.
    void actualizarHaloTempo(int ahoraMs, const sf::Vector2f& posMundoObjetivo);

    Player*   _player  = nullptr;
    Cancion*  _cancion = nullptr;
    Scoring*  _scoring = nullptr;

    float _celda = 0.f;
    float _calle = 0.f;

    int  _objetivoTiempoMs = 0;  // tiempo esperado de aterrizaje (ms)
    int  _indiceNota       = 0;  // 0 a 6 -> color de la guitarra / nodo correcto
    bool _objetivoActivo   = false;
    bool _nivelTerminado   = false;

    ConfigRitmo _cfg{};
    std::string _ultimoAcierto;
    sf::Clock   _relojJuego{};

    sf::Vector2i _posObjetivo{0, 0};       // nodo correcto
    sf::Vector2i _ultimaPos{0, 0};         // celda actual del jugador
    sf::Vector2i _celdaAnteriorReal{0, 0}; // de dónde venía realmente (para no generar atrás)

    // Nodos trampa (las otras direcciones válidas)
    std::vector<sf::Vector2i> _posTrampas;
    // Índice de nota (color) para cada trampa
    std::vector<int> _indicesNotasTrampa;

    sf::CircleShape _circuloObjetivo;
    sf::CircleShape _haloTempo;
    bool            _haloVisible = false;

    sf::Color _paletaNotas[7];
    sf::Font  _fuente;
    sf::Text  _texto;

    // Sistema de partículas para el nodo correcto (cuando acertás)
    ParticleSystemNodo _particleSystemNodo;
    // Partículas especiales del HALO cuando toca el nodo (momento PERFECT visual)
    ParticleSystemNodo _particleHalo;

    // Ayuda progresiva / reenganche (reducción de trampas según fallos)
    int  _fallosSeguidos  = 0;

    // Temblor de pantalla en errores
    float _shakeTimer     = 0.f;
    float _shakeDuration  = 0.f;
    float _shakeIntensity = 0.f;

    // Para no disparar la explosión de halo más de una vez por nota
    bool _haloExploto     = false;

    // Hit sounds
    sf::SoundBuffer _bufferHit;   // Perfect / Good
    sf::SoundBuffer _bufferBad;   // Bad
    sf::Sound       _sfxHit;
    sf::Sound       _sfxBad;
    bool _hitLoaded = false;
    bool _badLoaded = false;
};
