#pragma once
#include "Literals.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp" 


/**
 * @author Mar Werner Hernandez
 * @brief Custom extension to sf::SoundBuffer that allows one to see what sf::Sound instanced are attached to an sf::SoundBuffer instance
 */
namespace Parfait {
    struct SoundBuffer : public sf::SoundBuffer {
        /**
         * @brief Gets the set of pointers to sound instances which are currently using this SoundBuffer instance
         * 
         * @return const std::set<sf::Sound*>& 
         */
        const std::set<sf::Sound*>& getAttachedSounds(void);
    };
}

/**
 * @author Mar Werner Hernandez
 * @brief System used to manager audio playback
 * @note This is NOT a gamestate operator, its update function MUST be run after that of all gamestate operators
 */
class AudioManager {
    static constexpr inline u32 MAX_SOUNDS = 100;
    static constexpr inline u32 MAX_MUSIC_LAYERS = 3;
    static constexpr std::string AUDIO_DIRECTORY = "./assets/aud/";

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

            /**
             * @brief Pushes an element, which is expected to be [0, MAX_SOUNDS), to the back of the least-recently used queue of the SoundBuffers
             * 
             * @param element 
             */
            void Enqueue(const u8 element);

            /**
             * @brief Gets the index of the SoundBuffer in the soundBuffers array which has been used the least as of recent
             * 
             * @return u8 
             */
            u8 Dequeue(void);
        } leastRecentlyUsedQueue;

        sf::Music music[MAX_MUSIC_LAYERS];
        u32 musicHashes[MAX_MUSIC_LAYERS];
    };

    /**
     * @brief Gets the AudioManagerWrapper singleton
     * 
     * @return AudioManagerWrapper& 
     */
    static AudioManagerWrapper& GetAudioManagerWrapper(void);

    public:

    /**
     * @brief Initializes the audio manager
     *
     * @details Sets camera sounds and music to sound the same, regardless of the camera's position.
     *          Initializes least-recently used queue to expected conditions.
     *          Sets camera & positional sounds to not loop, and sets music to loop 
     * 
     */
    static void Initialize(void);

    /**
     * @brief Creates a camera Sound instance (sound sounds the same no matter the camera's position) for the desired sound, at the desired pitch (which affects playback speed)
     *
     * @details If the desired sound's waveform isn't already loaded in one of the SoundBuffers, the waveform is first loaded.
     *          The created sound instance is bound to its respective SoundBuffer
     * 
     * @param sound_name 
     * @param pitch 
     */
    static void StartCameraSound(const std::string&& sound_name, const f32 pitch = 1.0f);

    /**
     * @brief Creates a positional Sound instance (sound changes mixing based on its position relative to the camera's position) for the desired sound, at the desired pitch (which affects playback speed)
     *
     * @details If the desired sound's waveform isn't already loaded in one of the SoundBuffers, the waveform is first loaded.
     *          The created sound instance is bound to its respective SoundBuffer
     * 
     * @param sound_name 
     * @param pitch 
     */
    static void StartPositionalSound(const std::string&& sound_name, const sf::Vector3f& position, const f32 pitch = 1.0f);

    /**
     * @brief Starts a new looping stream for the desired music
     * 
     * @param music_name 
     * @param pitch 
     */
    static void StartMusic(const std::string&& music_name, const f32 pitch = 1.0f);

    /**
     * @brief Interrupts all camera Sound instances bound to the desired SoundBuffer
     * 
     * @param sound_name 
     */
    static void StopCameraSound(const std::string&& sound_name);

    /**
     * @brief Interrupts all positional Sound instances bound to the desired SoundBuffer
     * 
     * @param sound_name 
     */
    static void StopPositionalSound(const std::string&& sound_name);

    /**
     * @brief Interrupts streaming of the desired music
     * 
     * @param music_name 
     */
    static void StopMusic(const std::string&& music_name);

    /**
     * @brief Interrupts all sounds and music currently playing
     * 
     */
    static void ClearAudioQueues(void);

    /**
     * @brief Interrupts all playing camera Sounds
     * 
     */
    static void ClearCameraSounds(void);

    /**
     * @brief Interrupts all playing positional Sounds
     * 
     */
    static void ClearPositionalSounds(void);

    /**
     * @brief Interrupts all playing music
     * 
     */
    static void ClearMusic(void);

    /**
     * @brief Frees all positional and camera Sound instances that have played to completion for future use
     * 
     */
    static void Update(void);
};