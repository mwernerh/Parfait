#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManagerWrapper& AudioManager::GetAudioManagerWrapper(void) {
    static AudioManagerWrapper aM;
    return aM;
}

void AudioManager::Initialize(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();
    
    for(u32 i = 0; i < MAX_SOUNDS / 2; i++) {
        aM.cameraSounds[i].setBuffer(aM.soundBuffers[i]);
        aM.cameraSounds[i].setAttenuation(0.0f);
        aM.cameraSounds[i].setRelativeToListener(true);
        aM.cameraSounds[i].setLoop(false);

        aM.positionalSounds[i].setBuffer(aM.soundBuffers[(MAX_SOUNDS / 2) + i]);
        aM.positionalSounds[i].setAttenuation(0.8f);   // Not quite max attenuation
        aM.positionalSounds[i].setRelativeToListener(false);
        aM.positionalSounds[i].setLoop(false);
    }

    for(u32 i = 0; i < MAX_MUSIC_LAYERS; i++) {
        aM.music[i].setLoop(true);
        aM.music[i].setAttenuation(0.0f);
        aM.music[i].setRelativeToListener(true);
    }
}

void AudioManager::PlayCameraSound(const std::string&& sound_name, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    #ifdef DEBUG
    [[unlikely]]
    if(aM.numCameraSounds >= MAX_SOUNDS / 2) {
        std::cerr << "Error! Attempted to enqueue camera sound " << sound_name << " when buffer queue is full!\n";
        return;
    }
    #endif

    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        if(aM.soundNames[i] == sound_name && aM.cameraSounds[i].getPlayingOffset().asMilliseconds() == 0)
            return; // Sound has already been enqueued
    }

    aM.soundNames[aM.numCameraSounds] = sound_name;
    aM.soundBuffers[aM.numCameraSounds].loadFromFile(sound_name);
    aM.cameraSounds[aM.numCameraSounds].setPitch(pitch);
    aM.cameraSounds[aM.numCameraSounds++].play();
}

void AudioManager::PlayPositionalSound(const std::string&& sound_name, const sf::Vector3f& position, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    #ifdef DEBUG
    [[unlikely]]
    if(aM.numPositionalSounds >= MAX_SOUNDS / 2) {
        std::cerr << "Error! Attempted to enqueue positional sound " << sound_name << " when buffer queue is full!\n";
        return;
    }
    #endif

    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        if(aM.soundNames[(MAX_SOUNDS / 2) + i] == sound_name && aM.positionalSounds[i].getPlayingOffset().asMilliseconds() == 0)
            return; // Sound has already been enqueued
    }

    aM.soundNames[(MAX_SOUNDS / 2) + aM.numPositionalSounds] = sound_name;
    aM.soundBuffers[(MAX_SOUNDS / 2) + aM.numPositionalSounds].loadFromFile(sound_name);
    aM.positionalSounds[aM.numPositionalSounds].setPosition(position);
    aM.positionalSounds[aM.numPositionalSounds].setPitch(pitch);
    aM.positionalSounds[aM.numPositionalSounds++].play();
}

void AudioManager::PlayMusic(const std::string&& music_name, const f32 pitch) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();
    
    #ifdef DEBUG
    if(aM.numMusicLayers >= MAX_MUSIC_LAYERS) {
        std::cerr << "Error! Attempted to enqueue music " << music_name << " when stream layers are filled!\n";
        return;
    }
    #endif

    for(u32 i = 0; i < aM.numMusicLayers; i++) {
        if(aM.musicNames[i] == music_name)
            return; // Music has already been enqueued
    }

    aM.musicNames[aM.numMusicLayers] = music_name;
    aM.music[aM.numMusicLayers].openFromFile(music_name);
    aM.music[aM.numMusicLayers].setPitch(pitch);
    aM.music[aM.numMusicLayers++].play();
}

void AudioManager::StopCameraSound(const std::string&& sound_name) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        if(aM.soundNames[i] == sound_name) {
            aM.cameraSounds[i].stop();
            aM.soundBuffers[i].loadFromSamples(aM.soundBuffers[aM.numCameraSounds - 1].getSamples(),aM.soundBuffers[aM.numCameraSounds - 1].getSampleCount(), aM.soundBuffers[aM.numCameraSounds - 1].getChannelCount(), aM.soundBuffers[aM.numCameraSounds - 1].getSampleRate());
            aM.soundNames[i] = aM.soundNames[aM.numCameraSounds - 1];
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
        if(aM.soundNames[i] == sound_name) {
            aM.positionalSounds[i].stop();
            aM.soundBuffers[(MAX_SOUNDS / 2) + i].loadFromSamples(aM.soundBuffers[(MAX_SOUNDS / 2) + aM.numPositionalSounds - 1].getSamples(), aM.soundBuffers[(MAX_SOUNDS / 2) + aM.numPositionalSounds - 1].getSampleCount(), aM.soundBuffers[(MAX_SOUNDS / 2) + aM.numPositionalSounds - 1].getChannelCount(), aM.soundBuffers[(MAX_SOUNDS / 2) + aM.numPositionalSounds - 1].getSampleRate());
            aM.soundNames[(MAX_SOUNDS / 2) + i] = aM.soundNames[(MAX_SOUNDS / 2) + aM.numPositionalSounds - 1];
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
        if(aM.musicNames[i] == music_name) {
            aM.music[i].stop();
            aM.musicNames[i] = aM.musicNames[aM.numMusicLayers - 1];
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
    }
    aM.numCameraSounds = 0;
}

void AudioManager::ClearPositionalSounds(void) {
    AudioManagerWrapper& aM = GetAudioManagerWrapper();

    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        aM.positionalSounds[i].stop();
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

    for(u32 i = 0; i < aM.numCameraSounds; i++) {
        if(aM.cameraSounds[i].getStatus() == sf::Sound::Stopped)
            StopCameraSound(std::move(aM.soundNames[i]));
    }

    for(u32 i = 0; i < aM.numPositionalSounds; i++) {
        if(aM.positionalSounds[i].getStatus() == sf::Sound::Stopped)
            StopPositionalSound(std::move(aM.soundNames[(MAX_SOUNDS / 2) + i]));
    }
}