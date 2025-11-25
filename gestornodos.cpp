#include "GestorNodos.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <algorithm> //solo lo meti para minimos y maximos

GestorNodos::GestorNodos(float tamCelda, float tamCalle)
    : _celda(tamCelda), _calle(tamCalle)
{
    // Semilla básica para rand()
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Configuración inicial de texto de aciertos
    _fuente.loadFromFile("assets/fonts/comic.ttf");
    _texto.setFont(_fuente);
    _texto.setCharacterSize(30);
    _texto.setFillColor(sf::Color::Yellow);
    _texto.setOutlineColor(sf::Color::Black);
    _texto.setOutlineThickness(2.f);

    // nodo correcto, mas chico
    float r = 12.f;
    _circuloObjetivo.setRadius(r);
    _circuloObjetivo.setOrigin(r, r);

    // Nodo bien sólido y visible
    _circuloObjetivo.setFillColor(sf::Color::White); // se sobrescribe con el color de la nota
    _circuloObjetivo.setOutlineThickness(4.f);
    _circuloObjetivo.setOutlineColor(sf::Color(0, 0, 0, 230)); // borde negro

    // 🔵 Halo de tempo alrededor del nodo correcto:
    //    arranca grande y se cierra hasta el radio del nodo
    _haloTempo.setRadius(r * 2.8f);  // más grande para ver bien el cierre
    _haloTempo.setOrigin(_haloTempo.getRadius(), _haloTempo.getRadius());
    _haloTempo.setFillColor(sf::Color::Transparent);
    _haloTempo.setOutlineThickness(4.f);
    _haloTempo.setOutlineColor(sf::Color(255, 255, 255, 160));// blanco semi-transparente
    _haloVisible = false;

    _paletaNotas[0] = sf::Color(255,  64,  64); // rojo
    _paletaNotas[1] = sf::Color(255, 160,  64); // naranja
    _paletaNotas[2] = sf::Color(255, 240,  64); // amarillo
    _paletaNotas[3] = sf::Color( 64, 200,  64); // verde
    _paletaNotas[4] = sf::Color( 64, 160, 255); // celeste
    _paletaNotas[5] = sf::Color(160,  64, 255); // violeta
    _paletaNotas[6] = sf::Color(255,  64, 160); // rosado


    //sonidos de acierto y fallo
    if (_bufferHit.loadFromFile("assets/sounds/Perfect-Good.mp3"))
    {
        _sfxHit.setBuffer(_bufferHit);
        _sfxHit.setVolume(80.f);
        _hitLoaded = true;
    }
    else
    {
        std::cout << "No se pudo cargar sonido Perfect-Good.mp3" << std::endl;
    }

    if (_bufferBad.loadFromFile("assets/sounds/Bad.wav"))
    {
        _sfxBad.setBuffer(_bufferBad);
        _sfxBad.setVolume(80.f);
        _badLoaded = true;
    }
    else
    {
        std::cout << "No se pudo cargar sonido Bad.wav" << std::endl;
    }
}

//inicio
void GestorNodos::iniciar(Player* player, Cancion* cancion,
                          const ConfigRitmo& cfg, Scoring* scoring)
{
    _cfg = cfg;
    _relojJuego.restart();
    _player  = player;
    _cancion = cancion;
    _scoring = scoring;
    _nivelTerminado   = false;
    _ultimoAcierto.clear();
    _posTrampas.clear();
    _indicesNotasTrampa.clear();
    _haloVisible      = false;
    _fallosSeguidos   = 0;
    _shakeTimer       = 0.f;
    _shakeDuration    = 0.f;
    _shakeIntensity   = 0.f;
    _haloExploto      = false;

    if (_player != nullptr)
    {
        _player->restaurarVida();
        _player->reiniciarMovimiento();   // dejo el primer movimiento libre
        _ultimaPos         = _player->getPosGrilla();
        _celdaAnteriorReal = _ultimaPos;
    }

    if (_cancion != nullptr)
    {
        _cancion->reiniciar();
    }

    // Inicializamos la primera nota desde el CSV
    const NotaCancion* nota = (_cancion ? _cancion->getNotaActual() : nullptr);
    if (nota != nullptr)
    {
        _objetivoTiempoMs = static_cast<int>(nota->tiempoMs) + _cfg.offsetMs;
        _indiceNota       = nota->notaIndex;
        _objetivoActivo   = true;

        if (_player != nullptr)
        {
            // Generamos el nodo correcto + trampas
            generarObjetivoYTrampas(_player->getPosGrilla());
        }
        else
        {
            // Fallback
            _posObjetivo = nota->gridPos;
            _posTrampas.clear();
            _indicesNotasTrampa.clear();
        }
    }
    else
    {
        _objetivoActivo = false;
        _indiceNota = 0;
    }

    std::cout << "GestorNodos iniciado. "
              << "Objetivo en (" << _posObjetivo.x << "," << _posObjetivo.y
              << "), tiempo = " << _objetivoTiempoMs << "ms, nota = "
              << _indiceNota << std::endl;
}


// Actualizar (update + draw)

bool GestorNodos::actualizar(sf::RenderWindow& window, float dtSegundos)
{
    if (_nivelTerminado || _player == nullptr) return false;

    int ahora = _relojJuego.getElapsedTime().asMilliseconds();
    const sf::Vector2i posPlayer = _player->getPosGrilla();

    // Guardamos la vista original para poder restaurarla al final
    sf::View vistaOriginal = window.getView();

    // screen shake para los errores
    if (_shakeTimer > 0.f)
    {
        _shakeTimer -= dtSegundos;
        if (_shakeTimer < 0.f) _shakeTimer = 0.f;

        // Intensidad decae a medida que pasa el tiempo
        float tNorm = (_shakeDuration > 0.f) ? (_shakeTimer / _shakeDuration) : 0.f;
        float fuerza = _shakeIntensity * tNorm;

        // Offset aleatorio en X/Y
        float ox = (std::rand() / float(RAND_MAX) * 2.f - 1.f) * fuerza;
        float oy = (std::rand() / float(RAND_MAX) * 2.f - 1.f) * fuerza;

        sf::View shakeView = vistaOriginal;
        shakeView.move(ox, oy);
        window.setView(shakeView);
    }

    bool seguirJugando = true;

    // Actualizo los sistemas de partículas
    _particleSystemNodo.update(dtSegundos);
    _particleHalo.update(dtSegundos);

    // Si cambio de nodo, verifico como aterrizo.
    if (posPlayer != _ultimaPos)
    {
        // Guardo de donde viene el pj realmente (para no generar nodo atras)
        _celdaAnteriorReal = _ultimaPos;

        enAterrizajeJugador(posPlayer, ahora);
        _ultimaPos = posPlayer;
    }

    // Si se pasa de la ventana GOOD (tarde) sin acertar, es Bad automatico
    if (_objetivoActivo && ahora > (_objetivoTiempoMs + _cfg.ventanaGoodMsLate))
    {
        mostrarAcierto(aciertoGolpe::Bad, ahora - _objetivoTiempoMs);
        _player->reducirVida();
        avanzarANotaSiguiente();
    }

    // Dibujo el nodo objetivo (correcto) + trampas
    if (_objetivoActivo)
    {
        sf::Vector2f posMundo(_posObjetivo.x * _celda, _posObjetivo.y * _celda);

        //Actualizo el halo de tempo segun el tiempo actual del juego
        actualizarHaloTempo(ahora, posMundo);

        // primero el halo, atras del nodo correcto
        if (_haloVisible)
        {
            window.draw(_haloTempo);
        }

        //lo dejo fijo al nodo para ver el radio del halo
        _circuloObjetivo.setScale(1.f, 1.f);

        _circuloObjetivo.setPosition(posMundo);
        _circuloObjetivo.setFillColor(_paletaNotas[_indiceNota]); //mismo color que la guitarra
        window.draw(_circuloObjetivo);

        //Dibujo los nodos trampa (otras direcciones posibles)
        for (std::size_t i = 0; i < _posTrampas.size(); ++i)
        {
            const sf::Vector2i& pt = _posTrampas[i];
            sf::Vector2f posTrap(pt.x * _celda, pt.y * _celda);

            // Color de la trampa: usamos índice almacenado (si falta, caemos al de la nota correcta)
            int idxNotaTrampa = _indiceNota;
            if (i < _indicesNotasTrampa.size())
                idxNotaTrampa = _indicesNotasTrampa[i];

            sf::Color colTrap = _paletaNotas[idxNotaTrampa];

            //lo tiro hacia gris para que no compita tanto visualmente
            auto desaturar = [](sf::Color c, float factor) -> sf::Color {
                sf::Uint8 gray = static_cast<sf::Uint8>((c.r + c.g + c.b) / 3);
                sf::Uint8 r = static_cast<sf::Uint8>(gray + (c.r - gray) * factor);
                sf::Uint8 g = static_cast<sf::Uint8>(gray + (c.g - gray) * factor);
                sf::Uint8 b = static_cast<sf::Uint8>(gray + (c.b - gray) * factor);
                return sf::Color(r, g, b, c.a);
            };

            colTrap = desaturar(colTrap, 0.7f); // 0.0 = gris total, 1.0 = color original

            // Nodo trampa (más chico)
            sf::CircleShape trampa = _circuloObjetivo;
            trampa.setScale(0.85f, 0.85f); // un poco más chicas
            trampa.setPosition(posTrap);
            trampa.setFillColor(colTrap);
            trampa.setOutlineColor(sf::Color(0, 0, 0, 200));
            window.draw(trampa);

            // Halo de trampa
            //pero un poco más chico y más opaco
            const int anticipacionMs = 800;
            const int dashOffsetMs   = 180;
            const int afterExplodeMs = 200;

            int objetivoVisualMs  = _objetivoTiempoMs - dashOffsetMs;
            int tiempoHastaVisual = objetivoVisualMs - ahora;

            // Solo se dibuja halo si estamos dentro de la ventana
            if (tiempoHastaVisual <= anticipacionMs &&
                tiempoHastaVisual >= -afterExplodeMs)
            {
                sf::CircleShape haloTrampa;
                float radioTrap = trampa.getRadius();

                if (tiempoHastaVisual >= 0)
                {
                    // Antes del dash: halo grande que se va cerrando
                    float norm = 1.f - std::min(1.f, std::max(0.f,
                                        static_cast<float>(tiempoHastaVisual) /
                                        static_cast<float>(anticipacionMs)));

                    //aumento del tamaño del halo de trampa
                    float maxScaleTrap   = 2.4f;
                    float scaleTrap      = maxScaleTrap - (maxScaleTrap - 1.f) * norm;
                    float radioHaloTrap  = radioTrap * scaleTrap;

                    haloTrampa.setRadius(radioHaloTrap);
                    haloTrampa.setOrigin(radioHaloTrap, radioHaloTrap);
                    haloTrampa.setPosition(posTrap);
                    haloTrampa.setFillColor(sf::Color::Transparent);

                    // Un poco más visible que antes, pero menos que el correcto
                    sf::Color bordeTrap = colTrap;
                    bordeTrap.a = static_cast<sf::Uint8>(70 + 110 * norm);
                    haloTrampa.setOutlineColor(bordeTrap);

                    //Un poquito más grueso también
                    haloTrampa.setOutlineThickness(3.5f);

                    window.draw(haloTrampa);
                }
                else
                {
                    // el halo se queda cerca y se desvanece
                    int tLateMs = -tiempoHastaVisual;
                    float normLate = std::min(1.f,
                                       static_cast<float>(tLateMs) /
                                       static_cast<float>(afterExplodeMs));

                    float scaleTrap     = 1.f - 0.2f * normLate;
                    float radioHaloTrap = radioTrap * scaleTrap;

                    haloTrampa.setRadius(radioHaloTrap);
                    haloTrampa.setOrigin(radioHaloTrap, radioHaloTrap);
                    haloTrampa.setPosition(posTrap);
                    haloTrampa.setFillColor(sf::Color::Transparent);

                    sf::Color bordeTrap = colTrap;
                    bordeTrap.a = static_cast<sf::Uint8>(180 * (1.f - normLate));
                    haloTrampa.setOutlineColor(bordeTrap);
                    haloTrampa.setOutlineThickness(2.5f);

                    window.draw(haloTrampa);
                }
            }
        }
    }

    //partículas del nodo correcto
    _particleSystemNodo.draw(window);
    //partículas del halo (explosión de "perfect”)
    _particleHalo.draw(window);

    // Texto de feedback (Perfect / Good / Bad)
    dibujarStringAcierto(window);

    // Si se queda sin vida, termina la partida
    if (_player->getVida() == 0.f)
    {
        _nivelTerminado = true;
        seguirJugando   = false;
    }

    // Si ya no hay más notas en la cancion, marcamos nivel como completado
    if (_cancion != nullptr && _cancion->termino() && !_objetivoActivo)
    {
        if (_scoring != nullptr)
        {
            _scoring->setNivelCompletado();
        }
        _nivelTerminado = true;
        seguirJugando   = false;
    }

    // Restauramos SIEMPRE la vista original
    window.setView(vistaOriginal);

    return seguirJugando;
}

int GestorNodos::siguienteNota() const
{
    return _indiceNota;
}

// logica interna
void GestorNodos::avanzarANotaSiguiente()
{
    if (_cancion == nullptr)
    {
        _objetivoActivo = false;
        _haloVisible    = false;
        return;
    }

    // Pasamos a la siguiente entrada del CSV
    if (_cancion->avanzar())
    {
        const NotaCancion* nota = _cancion->getNotaActual();
        if (nota != nullptr)
        {
            _objetivoTiempoMs = static_cast<int>(nota->tiempoMs) + _cfg.offsetMs;
            _indiceNota       = nota->notaIndex;

            if (_player != nullptr)
            {
                generarObjetivoYTrampas(_player->getPosGrilla());
            }
            else
            {
                _posObjetivo = nota->gridPos;
                _posTrampas.clear();
                _indicesNotasTrampa.clear();
            }

            //  ayuda progrsiva para el jugador.
            //  0 fallos seguidos  -> normal (2 trampas)
            //  1 fallo seguido    -> 1 trampa
            //  2 o más fallos seg -> 0 trampas (solo el nodo correcto)
            if (_fallosSeguidos >= 1)
            {
                std::size_t trampasDeseadas;

                if (_fallosSeguidos == 1)
                    trampasDeseadas = 1;   // primer fallo -> 1 trampa
                else
                    trampasDeseadas = 0;   // segundo fallo o más -> 0 trampas

                if (_posTrampas.size() > trampasDeseadas)
                    _posTrampas.resize(trampasDeseadas);

                if (_indicesNotasTrampa.size() > trampasDeseadas)
                    _indicesNotasTrampa.resize(trampasDeseadas);
            }

            _objetivoActivo = true;
            _haloVisible    = false;
            _haloExploto    = false; // reseteamos explosión del halo para la nueva nota
            return;
        }
    }

    // Si no hay más notas
    _objetivoActivo = false;
    _haloVisible    = false;
}

void GestorNodos::enAterrizajeJugador(const sf::Vector2i& posActual, int ahoraMs)
{
    if (!_objetivoActivo || _cancion == nullptr) return;

    // Si no cayo en el objetivo es bad
    if (posActual != _posObjetivo)
    {
        mostrarAcierto(aciertoGolpe::Bad, 0);
        _player->reducirVida();
        avanzarANotaSiguiente();
        return;
    }

    // Delta firmado: negativo = se adelanto, positivo = se atraso
    int delta = ahoraMs - _objetivoTiempoMs;

    if (delta >= _cfg.ventanaPerfectMsEarly &&
        delta <= _cfg.ventanaPerfectMsLate)
    {
        mostrarAcierto(aciertoGolpe::Perfect, delta);
        _player->aumentarVida();
    }
    else if (delta >= _cfg.ventanaGoodMsEarly &&
             delta <= _cfg.ventanaGoodMsLate)
    {
        mostrarAcierto(aciertoGolpe::Good, delta);
        _player->aumentarVida();
    }
    else
    {
        mostrarAcierto(aciertoGolpe::Bad, delta);
        _player->reducirVida();
    }

    avanzarANotaSiguiente();
}

void GestorNodos::mostrarAcierto(aciertoGolpe acierto, int deltaMs)
{
    if (_scoring == nullptr) return;

    switch (acierto)
    {
    case aciertoGolpe::Perfect:
        _ultimoAcierto = "PERFECT!";
        _scoring->sumarPerfect();
        dispararParticulas(acierto);

        // (Perfect/Good comparten sample)
        if (_hitLoaded)
        {
            _sfxHit.stop();
            _sfxHit.play();
        }

        // contador de fallos, se recetea al acertar
        _fallosSeguidos = 0;
        break;

    case aciertoGolpe::Good:
        _ultimoAcierto = "GOOD";
        _scoring->sumarGood();
        dispararParticulas(acierto);

        // (Perfect/Good)
        if (_hitLoaded)
        {
            _sfxHit.stop();
            _sfxHit.play();
        }

        _fallosSeguidos = 0;
        break;

    case aciertoGolpe::Bad:
        _ultimoAcierto = "BAD";
        _scoring->sumarBad();

        // bad
        if (_badLoaded)
        {
            _sfxBad.stop();
            _sfxBad.play();
        }

        // si falla. sumamos al contador de fallos seguidos
        _fallosSeguidos++;

        // Temblor de pantalla al fallar
        _shakeDuration  = 0.18f;
        _shakeTimer     = _shakeDuration;
        _shakeIntensity = 6.f;

        // BAD no dispara partículas
        break;
    }

    _scoring->sumarMsAterrizaje(deltaMs);
    _scoring->sumarNotaAterrizada();
}

std::string GestorNodos::strConSigno(int tiempo) const
{
    if (tiempo > 0) return "+" + std::to_string(tiempo);
    return std::to_string(tiempo);
}

void GestorNodos::dibujarStringAcierto(sf::RenderWindow& window)
{
    if (_ultimoAcierto.empty()) return;

    sf::View view = window.getView();
    sf::Vector2f centro  = view.getCenter();
    sf::Vector2f tamanio = view.getSize();
    _texto.setString(_ultimoAcierto);
    _texto.setPosition(centro.x, centro.y - (tamanio.y * 0.20f));
    _texto.setOrigin(_texto.getLocalBounds().width * 0.5f,
                     _texto.getLocalBounds().height * 0.5f);
    window.draw(_texto);
}


// Generar nodo correcto + nodos trampa
void GestorNodos::generarObjetivoYTrampas(const sf::Vector2i& celdaBase)
{
    // Vecinos (derecha, izquierda, abajo, arriba)
    sf::Vector2i vecinos[4] =
    {
        {celdaBase.x + 1, celdaBase.y},     // derecha
        {celdaBase.x - 1, celdaBase.y},     // izquierda
        {celdaBase.x,     celdaBase.y + 1}, // abajo
        {celdaBase.x,     celdaBase.y - 1}  // arriba
    };

    // Construimos la lista de candidatos validos xq no volvemos a la celda de donde venimos
    std::vector<sf::Vector2i> candidatos;
    candidatos.reserve(4);
    for (int i = 0; i < 4; ++i)
    {
        if (vecinos[i] != _celdaAnteriorReal)
        {
            candidatos.push_back(vecinos[i]);
        }
    }

    if (candidatos.empty())
    {
        //si algo falla, nos quedamos con la primera dirección
        _posObjetivo = vecinos[0];
        _posTrampas.clear();
        _indicesNotasTrampa.clear();
        return;
    }

    // Elegimos un candidato al azar como nodo correcto
    int indiceCorrecto = std::rand() % candidatos.size();
    _posObjetivo = candidatos[indiceCorrecto];

    // El resto de candidatos se convierten en nodos trampa
    _posTrampas.clear();
    _indicesNotasTrampa.clear();
    for (int i = 0; i < static_cast<int>(candidatos.size()); ++i)
    {
        if (i == indiceCorrecto) continue;

        _posTrampas.push_back(candidatos[i]);

        // se elige un color de nota que no sea la nota real
        int idxFalso = std::rand() % 7;
        if (idxFalso == _indiceNota)
            idxFalso = (idxFalso + 1) % 7;

        _indicesNotasTrampa.push_back(idxFalso);
    }
}


// Partículas de acierto (nodo correcto)

void GestorNodos::dispararParticulas(aciertoGolpe acierto)
{
    if (!_objetivoActivo) return;

    // Posición en el mundo del nodo correcto
    sf::Vector2f posMundo(_posObjetivo.x * _celda, _posObjetivo.y * _celda);

    // Color base = color de la nota (mismo que el nodo/guitarra)
    sf::Color colorBase = _paletaNotas[_indiceNota];

    _particleSystemNodo.setOrigin(posMundo);
    _particleSystemNodo.setBaseColor(colorBase);

    // Config diferente según el tipo de acierto
    switch (acierto)
    {
    case aciertoGolpe::Perfect:
        // Muchos píxeles, más velocidad, explota fuerte
        _particleSystemNodo.triggerBurst(
            45,     // cantidad
            80.f,   // minSpeed
            200.f,  // maxSpeed
            0.25f,  // minLife (segundos)
            0.55f   // maxLife
        );
        break;

    case aciertoGolpe::Good:
        // Menos partículas, efecto más suave
        _particleSystemNodo.triggerBurst(
            22,     // cantidad
            60.f,   // minSpeed
            150.f,  // maxSpeed
            0.20f,  // minLife
            0.45f   // maxLife
        );
        break;

    case aciertoGolpe::Bad:
        // En BAD no disparamos partículas
        break;
    }
}


// Halo de tempo que se cierra hasta el radio del nodo
// explosión de partículas en el “momento justo”

void GestorNodos::actualizarHaloTempo(int ahoraMs, const sf::Vector2f& posMundoObjetivo)
{
    if (!_objetivoActivo)
    {
        _haloVisible = false;
        return;
    }

    // Tiempo máximo de anticipación del halo (ms antes de iniciar el dash)
    const int anticipacionMs = 800; // ~0.8s para que el ojo lea el círculo
    const int dashOffsetMs   = 180; // cuánto tarda el dash en llegar
    const int afterExplodeMs = 200; // duracion del brillo post-“perfect”

    //cuándo quiero que el jugador inicie el dash
    int objetivoVisualMs = _objetivoTiempoMs - dashOffsetMs;

    // Tiempo que falta para llegar al momento de inicio del dash
    int tiempoHastaVisual = objetivoVisualMs - ahoraMs;

    // Si está demasiado lejos en el futuro o demasiado pasado, no mostramos el halo
    if (tiempoHastaVisual > anticipacionMs ||
        tiempoHastaVisual < -afterExplodeMs)
    {
        _haloVisible = false;
        return;
    }

    _haloVisible = true;
    float radioNodo = _circuloObjetivo.getRadius();

    // Antes de llegar al momento visual: halo grande que se va cerrando
    if (tiempoHastaVisual >= 0)
    {
        // 0 es lejos, 1 es justo antes del dash
        float norm = 1.f - std::min(1.f, std::max(0.f,
                                static_cast<float>(tiempoHastaVisual) /
                                static_cast<float>(anticipacionMs)));

        float maxScale = 2.8f;         // arranca mucho más grande que el nodo
        float scale    = maxScale - (maxScale - 1.f) * norm; // se acerca a 1.0

        float radioHalo = radioNodo * scale;
        _haloTempo.setRadius(radioHalo);
        _haloTempo.setOrigin(radioHalo, radioHalo);
        _haloTempo.setPosition(posMundoObjetivo);

        // Color base del halo: mezcla entre blanco y el color de la nota
        sf::Color notaColor = _paletaNotas[_indiceNota];

        // norm ~ 0 (recién aparece) → halo casi blanco
        // norm ~ 1 (cerca del momento de dash) → halo del color de la nota
        float mix = norm;
        sf::Uint8 r = static_cast<sf::Uint8>(255 * (1.f - mix) + notaColor.r * mix);
        sf::Uint8 g = static_cast<sf::Uint8>(255 * (1.f - mix) + notaColor.g * mix);
        sf::Uint8 b = static_cast<sf::Uint8>(255 * (1.f - mix) + notaColor.b * mix);

        sf::Color c(r, g, b);

        // Alpha crece suave
        c.a = static_cast<sf::Uint8>(80 + 150 * norm);

        // Hacemos el halo del correcto un poquito más grueso cerca del momento justo
        float thickness = 3.f + 3.f * norm;
        _haloTempo.setOutlineThickness(thickness);

        _haloTempo.setOutlineColor(c);
    }
    else
    {
        // Ya pasamos el momento visual (el halo ya "toco" el nodo)
        // tiempoHastaVisual es negativo en este tramo
        int tLateMs = -tiempoHastaVisual;
        float normLate = std::min(1.f,
                           static_cast<float>(tLateMs) /
                           static_cast<float>(afterExplodeMs));

        // El halo se queda cerca del tamaño del nodo y se desvanece
        float scale = 1.f - 0.2f * normLate;
        float radioHalo = radioNodo * scale;
        _haloTempo.setRadius(radioHalo);
        _haloTempo.setOrigin(radioHalo, radioHalo);
        _haloTempo.setPosition(posMundoObjetivo);

        sf::Color c = _haloTempo.getOutlineColor();
        c.a = static_cast<sf::Uint8>(230 * (1.f - normLate));
        _haloTempo.setOutlineColor(c);

        //justo cuando el halo "toca" el nodo (instante PERFECT visual)
        if (!_haloExploto)
        {
            // Solo disparamos la explosión cuando recien cruzamos el momento visual
            // (primeros ~30ms despues)
            if (tLateMs <= 30)
            {
                sf::Color base = _paletaNotas[_indiceNota];

                _particleHalo.setOrigin(posMundoObjetivo);
                _particleHalo.setBaseColor(base);

                _particleHalo.triggerBurst(
                    28,     // cantidad de partículas
                    70.f,   // minSpeed
                    170.f,  // maxSpeed
                    0.20f,  // minLife
                    0.45f   // maxLife
                );
            }

            _haloExploto = true;
        }
    }
}
