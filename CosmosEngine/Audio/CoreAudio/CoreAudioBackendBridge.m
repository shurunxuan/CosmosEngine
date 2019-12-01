#import "CoreAudioBackendBridge.h"
#import "../../Logging/LoggingObjC.h"
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

//NSAutoreleasePool* pool;
//AVAudioPlayerNode* playerNode;
//AVAudioFormat* format;

void CreateEngine(AVAudioEngine** engine)
{
    *engine = [[AVAudioEngine alloc] init];
    LOG_INFO("AVAudioEngine created");
}