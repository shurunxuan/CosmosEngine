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
}

void GetMainMixerNodeFromEngine(AVAudioEngine* engine, AVAudioMixerNode** mixerNode)
{
    *mixerNode = [engine mainMixerNode];
}

void StartEngine(AVAudioEngine* engine)
{
    NSError* error;
    if (![engine startAndReturnError:&error])
    {
        const char* errStr = [[NSString stringWithFormat:@"%@", error] UTF8String];
        LOG_ERROR(errStr);
    }
    LOG_INFO("AVAudioEngine started");
}

void StopEngine(AVAudioEngine* engine)
{
    [engine stop];
    LOG_INFO("AVAudioEngine stopped");
}

void CreatePlayerNode(AVAudioPlayerNode** playerNode)
{
    *playerNode = [[AVAudioPlayerNode alloc] init];
}

void AttachNodeToEngine(AVAudioNode* node, AVAudioEngine* engine)
{
    [engine attachNode:node];
}

void CreateFormat(AVAudioFormat** format, int sampleRate, int channels)
{
    *format = [[AVAudioFormat alloc] initWithCommonFormat:AVAudioPCMFormatFloat32
               sampleRate:sampleRate channels:(AVAudioChannelCount) channels interleaved:false];
}

void ConnectNodeToNodeWithFormat(AVAudioEngine* engine,
                                 AVAudioNode* source, AVAudioNode* target, AVAudioFormat* format)
{
    [engine connect:source to:target format:format];
}

void StartPlayerNode(AVAudioPlayerNode* node)
{
    [node play];
}

void PausePlayerNode(AVAudioPlayerNode* node)
{
    [node pause];
}

void StopPlayerNode(AVAudioPlayerNode* node)
{
    [node stop];
}

void SetupAudioBuffer(unsigned char* buffer, int bufferSize,
        AVAudioPlayerNode* playerNode, AVAudioFormat* format, void(*Callback)(void*), void* callbackData)
{
    const size_t sizeConv = sizeof(float) / sizeof(unsigned char);
    const size_t fBufferSize = bufferSize / sizeConv;
    const int channels = [format channelCount];
    AVAudioPCMBuffer* avAudioBuffer = [[AVAudioPCMBuffer alloc] initWithPCMFormat:format
                                                                    frameCapacity:(AVAudioFrameCount) (fBufferSize / channels)];

    avAudioBuffer.frameLength = (AVAudioFrameCount) (fBufferSize / channels);

    float* fBuffer = (float*)buffer;

    for (int i = 0; i < fBufferSize; ++i)
    {
        avAudioBuffer.floatChannelData[i % channels][i / channels] = fBuffer[i];
    }

    [playerNode scheduleBuffer:avAudioBuffer completionHandler:^{
        Callback(callbackData);
    }];
}