//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_AUDIOSOURCE_H
#define GAMEENGINE_AUDIOSOURCE_H

#include "../Export.h"
#include "../Audio/DefaultAudioBackend.h"
#include "Component.h"
#include <boost/thread.hpp>
#include <boost/container/string.hpp>

class ENGINE_API AudioSource : public Component
{
public:
    explicit AudioSource(Object* owner);

    ~AudioSource() final;

    /**
     * @brief Will be called as soon as the component is created
     *
     */
    void Start() final;

    /**
     * @brief Will be called when the object updates
     *
     * @param deltaTime The time that a frame costs
     * @param totalTime The total time from the beginning of the application
     */
    void Update(float deltaTime, float totalTime) final;

    /**
     * @brief Load an audio file
     *
     * @param filename The audio file name
     */
    void LoadAudioFile(const boost::container::string& filename);

    /**
     * @brief Start/resume the audio playback
     *
     */
    void Play();
    /**
     * @brief Stop the audio playback completely
     *
     */
    void Stop();
    /**
     * @brief Pause the audio playback
     *
     */
    void Pause();

    /**
     * @brief Indicates if the audio is playing
     *
     * @return true if the audio is playing
     */
    bool Playing() const;
    /**
     * @brief Indicates if the audio is completely stopped
     *
     * @return true if the audio is completely stopped
     */
    bool Stopped() const;
    /**
     * @brief Indicates if the audio is paused
     *
     * @return true if the audio is paused
     */
    bool Paused() const;

    bool Loop;

private:

    AudioPlayer* player;

    Decoder ffmpeg;

    boost::thread playbackThread;

    /**
     * @brief Indicates if the audio is playing
     *
     */
    bool isPlaying;
    /**
     * @brief Indicates if the audio is completely stopped
     *
     */
    bool stopped;
    /**
     * @brief Indicates if the audio file is opened
     *
     */
    bool fileOpened;

    boost::container::string filename;

    int channels;

    int sampleRate;

    int bytesPerSample;

    /**
     * @brief The synchronized audio playback function
     *
     */
    void PlaySync();
};

#endif //GAMEENGINE_AUDIOSOURCE_H
