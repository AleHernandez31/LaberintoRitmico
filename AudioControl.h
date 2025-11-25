#pragma once
#include <SFML/Audio.hpp>
#include <string>

class AudioControl {
    public:
        AudioControl(std::string path, bool isMenu);

        /// Re-carga otro archivo de audio usando la misma instancia
        void load(const std::string& path);

        void play();
        void stop();
        void pause();
        void setVolume(float volume);

    private:
        sf::Music _music;
        sf::Sound _sound;
        sf::SoundBuffer _soundBuffer;
        bool _isMenu;
};
