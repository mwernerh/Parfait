#include "Framework_Managers/AudioManager.h"
#include <cstring>
#include <iostream>

/**
 * @author Mar Werner Hernandez
 */

namespace {
    /**
     * @brief Hashes the relative path to the desired audio file
     * 
     * @param string 
     * @return constexpr u32 
     */
    constexpr u32 GetHash(const std::string& string) {
        u32 hash = 0;
        u32 directoryLevel = 0;

        // Use prime numbers for weights
        constexpr u32 weights[] = {1, 17, 19, 23, 29, 31, 37};

        for(std::size_t i = string.length(); i > 0; i--) {
            if(string[i - 1] == '/') {
                directoryLevel++;
                continue;
            }

            hash += string[i - 1] * weights[directoryLevel];
        }

        return hash;
    }
}

const std::set<sf::Sound*>& Parfait::SoundBuffer::getAttachedSounds(void) {
    // Hack to allow access to the private member variable m_sounds, for which no public accessor was provided!!
    // It's silly that we can't see which sf::Sound instances are bound to a specific audio buffer...

    sf::SoundBuffer* hack = std::bit_cast<sf::SoundBuffer*>(this);
    hack += 1;

    u8* hack2 = std::bit_cast<u8*>(hack);
    hack2 -= sizeof(std::set<sf::Sound*>);

    return *std::bit_cast<std::set<sf::Sound*>*>(hack2); 
}

void AudioManager::AudioManagerWrapper::LRUQueue::Enqueue(const u8 element) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    // Place the desired element at the back of the queue

    for(u32 i = 0; i < MAX_SOUNDS; i++) {
        if(aM.leastRecentlyUsedQueue.array[(aM.leastRecentlyUsedQueue.front + i) % MAX_SOUNDS] == element) {
            for(u32 j = i; j < MAX_SOUNDS - 1; j++) {
                aM.leastRecentlyUsedQueue.array[(aM.leastRecentlyUsedQueue.front + i) % MAX_SOUNDS] = aM.leastRecentlyUsedQueue.array[(aM.leastRecentlyUsedQueue.front + i + 1) % MAX_SOUNDS];
            }
            break;
        }
    }

    aM.leastRecentlyUsedQueue.array[(aM.leastRecentlyUsedQueue.front + MAX_SOUNDS - 1) % MAX_SOUNDS] = element;
}

u8 AudioManager::AudioManagerWrapper::LRUQueue::Dequeue(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();
    
    // Get element at front of queue
    u8 element = aM.leastRecentlyUsedQueue.array[aM.leastRecentlyUsedQueue.front];
    aM.leastRecentlyUsedQueue.front = (aM.leastRecentlyUsedQueue.front + 1) % MAX_SOUNDS;

    return element;
}

AudioManager::AudioManagerWrapper& AudioManager::GetAudioManagerWrapper(void) {
    static AudioManagerWrapper aM;
    return aM;
}

void AudioManager::Initialize(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();
    
    for(u32 i = 0; i < MAX_SOUNDS / 2; i++) {
        // Camera sounds sound the same no matter camera position, and do not loop
        aM.cameraSounds[i].setAttenuation(0.0f);
        aM.cameraSounds[i].setRelativeToListener(true);
        aM.cameraSounds[i].setLoop(false);

        // Positional sounds sound differently, depending on the camera position, and do not loop
        aM.positionalSounds[i].setAttenuation(0.5f);   // Not quite max attenuation
        aM.positionalSounds[i].setRelativeToListener(false);
        aM.positionalSounds[i].setMinDistance(550.0f);
        aM.positionalSounds[i].setLoop(false);

    }

    // Music sounds the same no matter the camera position, and loops
    for(u32 i = 0; i < MAX_MUSIC_LAYERS; i++) {
        aM.music[i].setAttenuation(0.0f);
        aM.music[i].setRelativeToListener(true);
        aM.music[i].setLoop(true);
    }

    // Set LRU queue to expected values
    for(u32 i = 0; i < MAX_SOUNDS; i++) {
        aM.leastRecentlyUsedQueue.array[i] = i;
    }

    // Initialize relevant member attributes
    aM.leastRecentlyUsedQueue.front = 0;
    aM.numCameraSounds = 0;
    aM.numPositionalSounds = 0;
    aM.numMusicLayers = 0;
    aM.numLoadedSoundBuffers = 0;
}

void AudioManager::StartCameraSound(const std::string&& sound_name, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    #ifdef DEBUG
    [[unlikely]]
    if(aM.numCameraSounds >= MAX_SOUNDS / 2) {
        std::cerr << "\x1b[31mError! Attempted to enqueue camera sound " << sound_name << " when queue is full!\x1b[0m\n";
        return;
    }
    #endif

    sf::SoundBuffer* buffer = nullptr;

    for(u32 i = 0; i < aM.numLoadedSoundBuffers; i++) {
        // Sound buffer with this sound has already been loaded
        if(aM.soundHashes[i] == GetHash(sound_name)) {
            for(sf::Sound* sound : aM.soundBuffers[i].getAttachedSounds()) {
                if(sound >= aM.cameraSounds && sound < (aM.cameraSounds + MAX_SOUNDS / 2) && sound->getPlayingOffset().asMilliseconds() == 0) {
                    aM.leastRecentlyUsedQueue.Enqueue(i);
                    return; // The requested sound has been enqueued previously -- don't double up
                }
            }

            // This is a unique sound, and has an already loaded sound buffer
            buffer = &aM.soundBuffers[i];
        }
    }

    if(buffer) {
        // Requested sound has previously been loaded, just register the new sound's buffer as this buffer
        aM.cameraSounds[aM.numCameraSounds].setBuffer(*buffer);
    }
    else {
        // Requested sound has not been loaded -- load its buffer
        buffer = &aM.soundBuffers[aM.leastRecentlyUsedQueue.Dequeue()];
        aM.soundHashes[aM.numLoadedSoundBuffers] = GetHash(sound_name);

        if(!aM.soundBuffers[aM.numLoadedSoundBuffers].loadFromFile(AUDIO_DIRECTORY + sound_name + std::string(".wav"))) {
            std::cerr << "\x1b[31mError! Could not load sound file " << sound_name << " when preparing a SoundBuffer!\x1b[0m\n";
            return;
        }

        aM.cameraSounds[aM.numCameraSounds].setBuffer(*buffer);
        aM.numLoadedSoundBuffers += 1;
    }
    
    aM.cameraSounds[aM.numCameraSounds].setPitch(pitch);
    aM.cameraSounds[aM.numCameraSounds].play();
    aM.numCameraSounds += 1;
}

void AudioManager::StartPositionalSound(const std::string&& sound_name, const sf::Vector3f& position, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    #ifdef DEBUG
    [[unlikely]]
    if(aM.numPositionalSounds >= MAX_SOUNDS / 2) {
        std::cerr << "\x1b[31mError! Attempted to enqueue positional sound " << sound_name << " when queue is full!\x1b[0m\n";
        return;
    }
    #endif

    sf::SoundBuffer* buffer = nullptr;

    for(u32 i = 0; i < aM.numLoadedSoundBuffers; i++) {
        // Sound buffer with this sound has already been loaded
        if(aM.soundHashes[i] == GetHash(sound_name)) {
            for(sf::Sound* sound : aM.soundBuffers[i].getAttachedSounds()) {
                if(sound >= aM.positionalSounds && sound <= (aM.positionalSounds + MAX_SOUNDS / 2) && sound->getPlayingOffset().asMilliseconds() == 0) {
                    aM.leastRecentlyUsedQueue.Enqueue(i);
                    return; // The requested sound has been enqueued previously -- don't double up
                }
            }

            // This is a unique sound, and has an already loaded sound buffer
            buffer = &aM.soundBuffers[i];
        }
    }

    if(buffer) {
        // Requested sound has previously been loaded, just register the new sound's buffer as this buffer
        aM.positionalSounds[aM.numPositionalSounds].setBuffer(*buffer);
    }
    else {
        // Requested sound has not been loaded -- load its buffer
        buffer = &aM.soundBuffers[aM.leastRecentlyUsedQueue.Dequeue()];
        aM.soundHashes[aM.numLoadedSoundBuffers] = GetHash(sound_name);

        if(!aM.soundBuffers[aM.numLoadedSoundBuffers].loadFromFile(AUDIO_DIRECTORY + sound_name + std::string(".wav"))) {
            std::cerr << "\x1b[31mError! Could not load sound file " << sound_name << " when preparing a SoundBuffer!\x1b[0m\n";
            return;
        }

        aM.positionalSounds[aM.numPositionalSounds].setBuffer(*buffer);
        aM.numLoadedSoundBuffers += 1;
    }
    
    aM.positionalSounds[aM.numPositionalSounds].setPitch(pitch);
    aM.positionalSounds[aM.numPositionalSounds].setPosition(position);
    aM.positionalSounds[aM.numPositionalSounds].play();
    aM.numPositionalSounds += 1;
}

void AudioManager::StartMusic(const std::string&& music_name, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();
    
    #ifdef DEBUG
    if(aM.numMusicLayers >= MAX_MUSIC_LAYERS) {
        std::cerr << "\x1b[31mError! Attempted to enqueue music " << music_name << " when stream layers are filled!\x1b[0m\n";
        return;
    }
    #endif

    for(u32 i = 0; i < aM.numMusicLayers; i++) {
        if(aM.musicHashes[i] == GetHash(music_name))
            return; // Music has already been enqueued
    }

    aM.musicHashes[aM.numMusicLayers] = GetHash(music_name);

    if(!aM.music[aM.numMusicLayers].openFromFile(AUDIO_DIRECTORY + music_name + std::string(".wav"))) {
        std::cerr << "\x1b[31mError! Could not load music file " << music_name << " when preparing an audio stream!\x1b[0m\n";
        return;
    }

    aM.music[aM.numMusicLayers].setPitch(pitch);
    aM.music[aM.numMusicLayers++].play();
}

void AudioManager::StopCameraSound(const std::string&& sound_name) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        if(aM.soundHashes[i] == GetHash(sound_name)) {
            aM.cameraSounds[i].resetBuffer();
            aM.numCameraSounds -= 1;
            return;
        }
    }

    #ifdef DEBUG
    std::cerr << "Could not find " << sound_name << " to stop in camera sound queue!\n";
    #endif
}

void AudioManager::StopPositionalSound(const std::string&& sound_name) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        if(aM.soundHashes[i] == GetHash(sound_name)) {
            aM.positionalSounds[i].resetBuffer();
            aM.numPositionalSounds -= 1;
            return;
        }
    }

    #ifdef DEBUG
    std::cerr << "Could not find " << sound_name << " to stop in positional sound queue!\n";
    #endif
}

void AudioManager::StopMusic(const std::string&& music_name) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numMusicLayers; i++) {
        if(aM.musicHashes[i] == GetHash(music_name)) {
            aM.music[i].stop();
            aM.numMusicLayers -= 1;
            return;
        }
    }

    #ifdef DEBUG
    std::cerr << "Could not find " << music_name << " to stop in music layers!\n";
    #endif
}



void AudioManager::ClearAudioQueues(void) {
    ClearCameraSounds();
    ClearPositionalSounds();
    ClearMusic();
}

void AudioManager::ClearCameraSounds(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        aM.cameraSounds[i].stop();
        aM.cameraSounds[i].resetBuffer();
    }
    aM.numCameraSounds = 0;
}

void AudioManager::ClearPositionalSounds(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        aM.positionalSounds[i].stop();
        aM.positionalSounds[i].resetBuffer();
    }
    aM.numPositionalSounds = 0;
}

void AudioManager::ClearMusic(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numMusicLayers; i++) {
        aM.music[i].stop();
    }
    aM.numMusicLayers = 0;
}


void AudioManager::Update(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    // Check if any camera sounds have reached completion... if so, unbind it from any SoundBuffer
    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        if(aM.cameraSounds[i].getStatus() == sf::Sound::Stopped) {
            aM.cameraSounds[i].stop();
            aM.cameraSounds[i].resetBuffer();
            aM.numCameraSounds -= 1;
        }
    }

    // Check if any positional sounds have reached completion... if so, unbind it from any SoundBuffer
    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        if(aM.positionalSounds[i].getStatus() == sf::Sound::Stopped) {
            aM.positionalSounds[i].resetBuffer();
            aM.numPositionalSounds -= 1;
        }
    }

    // Find the first camera sound that is unbound
    u32 firstEmptyIdx = 0;
    for(; firstEmptyIdx < MAX_SOUNDS / 2; firstEmptyIdx++) {
        if(aM.cameraSounds[firstEmptyIdx].getBuffer() == nullptr) {
            break;
        }
    }

    // Fill the empty found index, repeating for any later found empty indices
    for(u32 j = firstEmptyIdx + 1; j < MAX_SOUNDS / 2; j++) {
        const sf::SoundBuffer* const currentBuffer = aM.cameraSounds[j].getBuffer();
        if(currentBuffer) {
            aM.cameraSounds[firstEmptyIdx].pause();
            aM.cameraSounds[firstEmptyIdx].setBuffer(*currentBuffer);
            aM.cameraSounds[firstEmptyIdx].setPlayingOffset(aM.cameraSounds[j].getPlayingOffset());
            aM.cameraSounds[firstEmptyIdx].setPitch(aM.cameraSounds[j].getPitch());
            aM.cameraSounds[firstEmptyIdx].play();
            aM.cameraSounds[j].resetBuffer();
            firstEmptyIdx = j;
        }
    }

    for(firstEmptyIdx = 0; firstEmptyIdx < MAX_SOUNDS / 2; firstEmptyIdx++) {
        if(aM.positionalSounds[firstEmptyIdx].getBuffer() == nullptr) {
            break;
        }
    }

    for(u32 j = firstEmptyIdx + 1; j < MAX_SOUNDS / 2; j++) {
        const sf::SoundBuffer* const currentBuffer = aM.positionalSounds[j].getBuffer();
        if(currentBuffer) {
            aM.positionalSounds[firstEmptyIdx].pause();
            aM.positionalSounds[firstEmptyIdx].setBuffer(*currentBuffer);
            aM.positionalSounds[firstEmptyIdx].setPlayingOffset(aM.positionalSounds[j].getPlayingOffset());
            aM.positionalSounds[firstEmptyIdx].setPitch(aM.cameraSounds[j].getPitch());
            aM.positionalSounds[firstEmptyIdx].setPosition(aM.cameraSounds[j].getPosition());
            aM.positionalSounds[firstEmptyIdx].play();
            aM.positionalSounds[j].resetBuffer();
            firstEmptyIdx = j;
        }
    }
}