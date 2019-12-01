//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_LOGGINGOBJC_H
#define GAMEENGINE_LOGGINGOBJC_H

#ifdef __cplusplus
extern "C"
{
#endif

    void LogTrace(const char* str, int line, const char* file);
    void LogDebug(const char* str, int line, const char* file);
    void LogInfo(const char* str, int line, const char* file);
    void LogWarning(const char* str, int line, const char* file);
    void LogError(const char* str, int line, const char* file);
    void LogFatal(const char* str, int line, const char* file);

#ifdef __OBJC__
#define LOG_TRACE(str) LogTrace(str, __LINE__, __FILE__)
#define LOG_DEBUG(str) LogDebug(str, __LINE__, __FILE__)
#define LOG_INFO(str) LogInfo(str, __LINE__, __FILE__)
#define LOG_WARNING(str) LogWarning(str, __LINE__, __FILE__)
#define LOG_ERROR(str) LogError(str, __LINE__, __FILE__)
#define LOG_FATAL(str) LogFatal(str, __LINE__, __FILE__)
#endif

#ifdef __cplusplus
}
#endif

#endif //GAMEENGINE_LOGGINGOBJC_H
