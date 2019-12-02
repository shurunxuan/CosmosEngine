//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_COREAUDIOBACKENDBRIDGE_H
#define GAMEENGINE_COREAUDIOBACKENDBRIDGE_H

#include <objc/objc-runtime.h>
#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    // Objective-C
    OBJC_CLASS(AVAudioEngine);
    OBJC_CLASS(AVAudioNode);
    OBJC_CLASS(AVAudioPlayerNode);
    OBJC_CLASS(AVAudioFormat);
    OBJC_CLASS(AVAudioMixerNode);
    OBJC_CLASS(AVAudioFormat);

//    void InitPool();
//
//    void DrainPool();

    void CreateEngine(AVAudioEngine** engine);

    void GetMainMixerNodeFromEngine(AVAudioEngine* engine, AVAudioMixerNode** mixerNode);

    void StartEngine(AVAudioEngine* engine);

    void StopEngine(AVAudioEngine* engine);

    void CreatePlayerNode(AVAudioPlayerNode** playerNode);

    void AttachNodeToEngine(AVAudioNode* node, AVAudioEngine* engine);

    void CreateFormat(AVAudioFormat** format, int sampleRate, int channels);

    void ConnectNodeToNodeWithFormat(AVAudioEngine* engine,
            AVAudioNode* source, AVAudioNode* target, AVAudioFormat* format);

    void StartPlayerNode(AVAudioPlayerNode* node);

    void PausePlayerNode(AVAudioPlayerNode* node);

    void StopPlayerNode(AVAudioPlayerNode* node);

    void SetupAudioBuffer(unsigned char* buffer, int bufferSize,
                          AVAudioPlayerNode* playerNode, AVAudioFormat* format, void(*Callback)(void*), void* callbackData);

#ifdef __cplusplus
}
#endif

#endif //GAMEENGINE_COREAUDIOBACKENDBRIDGE_H
