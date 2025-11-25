#include <string>
#include <iostream>
#include "AudioControl.h"

/*
    Parametros:
        path: ruta de la cancion (mp3, ogg, wav, etc.)
        isMenu: si es true usamos sf::Music (stream desde disco, ideal para musica de fondo),
                si es false usamos sf::Sound (carga completa en memoria, ideal para SFX cortos)
*/
AudioControl::AudioControl(std::string path, bool isMenu)
    : _isMenu(isMenu)
{
    load(path);
}

void AudioControl::load(const std::string& path)
{
    if (_isMenu)
    {
        // Musica de fondo (menu, musica larga)
        if (!_music.openFromFile(path))
        {
            std::cout << "Error al cargar musica: " << path << std::endl;
            return;
        }
        _music.setLoop(true); // musica de fondo en loop
    }
    else
    {
        // Sonido via SoundBuffer (en tu caso la estas usando para la musica del juego)
        if (!_soundBuffer.loadFromFile(path))
        {
            std::cout << "Error al cargar sonido: " << path << std::endl;
            return;
        }
        _sound.setBuffer(_soundBuffer);
    }
}

void AudioControl::play()
{
    if (_isMenu) _music.play();
    else         _sound.play();
}

void AudioControl::stop()
{
    if (_isMenu) _music.stop();
    else         _sound.stop();
}

void AudioControl::pause()
{
    if (_isMenu) _music.pause();
    else         _sound.pause();
}

void AudioControl::setVolume(float volume)
{
    if (_isMenu) _music.setVolume(volume);
    else         _sound.setVolume(volume);
}
