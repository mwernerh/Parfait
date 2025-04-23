#pragma once
#include "Literals.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp" 

namespace Parfait {
    struct SoundBuffer : public sf::SoundBuffer {
        const std::set<sf::Sound*>& getAttachedSounds(void);
    };
}

class AudioManager {
    static constexpr inline u32 MAX_SOUNDS = 100;
    static constexpr inline u32 MAX_MUSIC_LAYERS = 3;

    struct AudioManagerWrapper {
        u32 numCameraSounds;
        u32 numPositionalSounds;
        u32 numMusicLayers;
        u32 numLoadedSoundBuffers;

        sf::Sound cameraSounds[MAX_SOUNDS / 2];
        sf::Sound positionalSounds[MAX_SOUNDS / 2];
        Parfait::SoundBuffer soundBuffers[MAX_SOUNDS];
        u32 soundHashes[MAX_SOUNDS];
        
        struct LRUQueue {
            u8 array[MAX_SOUNDS];
            u8 front;

            void Enqueue(const u8 element);
            u8 Dequeue(void);
        } leastRecentlyUsedQueue;

        sf::Music music[MAX_MUSIC_LAYERS];
        u32 musicHashes[MAX_MUSIC_LAYERS];
    };

    static AudioManagerWrapper& GetAudioManagerWrapper(void);

    public:
    static void Initialize(void);

    static void StartCameraSound(const std::string&& sound_name, const f32 pitch = 1.0f);
    static void StartPositionalSound(const std::string&& sound_name, const sf::Vector3f& position, const f32 pitch = 1.0f);
    static void StartMusic(const std::string&& music_name, const f32 pitch = 1.0f);

    static void StopCameraSound(const std::string&& sound_name);
    static void StopPositionalSound(const std::string&& sound_name);
    static void StopMusic(const std::string&& music_name);
    static void StopCameraSound(const u32&& sound_hash);
    static void StopPositionalSound(const u32&& sound_hash);
    static void StopMusic(const u32&& music_hash);

    static void ClearAudioQueues(void);
    static void ClearCameraSounds(void);
    static void ClearPositionalSounds(void);
    static void ClearMusic(void);

    static void Update(void);
};