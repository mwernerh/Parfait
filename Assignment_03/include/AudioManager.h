#pragma once
#include "Literals.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

class AudioManager {
    static constexpr inline u32 MAX_SOUNDS = 100;
    static constexpr inline u32 MAX_MUSIC_LAYERS = 3;

    struct AudioManagerWrapper {
        u32 numCameraSounds;
        u32 numPositionalSounds;
        u32 numMusicLayers;
        sf::Sound cameraSounds[MAX_SOUNDS / 2];
        sf::Sound positionalSounds[MAX_SOUNDS / 2];
        sf::SoundBuffer soundBuffers[MAX_SOUNDS];
        std::string soundNames[MAX_SOUNDS];
        sf::Music music[MAX_MUSIC_LAYERS];
        std::string musicNames[MAX_MUSIC_LAYERS];
    };

    static AudioManagerWrapper& GetAudioManagerWrapper(void);

    public:
    static void Initialize(void);

    static void PlayCameraSound(const std::string&& sound_name, const f32 pitch = 1.0f);
    static void PlayPositionalSound(const std::string&& sound_name, const sf::Vector3f& position, const f32 pitch = 1.0f);
    static void PlayMusic(const std::string&& music_name, const f32 pitch = 1.0f);

    static void StopCameraSound(const std::string&& sound_name);
    static void StopPositionalSound(const std::string&& sound_name);
    static void StopMusic(const std::string&& music_name);

    static void ClearAudioQueues(void);
    static void ClearCameraSounds(void);
    static void ClearPositionalSounds(void);
    static void ClearMusic(void);

    static void Update(void);
};