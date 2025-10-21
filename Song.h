#pragma once
#include <SFML/Audio.hpp>

class Song {
    public:
        Song(std::string path, bool isMenu);
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
