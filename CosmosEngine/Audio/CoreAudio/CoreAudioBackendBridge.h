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

//    void InitPool();
//
//    void DrainPool();

    void CreateEngine(AVAudioEngine** engine);

#ifdef __cplusplus
}
#endif

#endif //GAMEENGINE_COREAUDIOBACKENDBRIDGE_H
