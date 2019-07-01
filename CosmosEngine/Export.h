//
// Created by 西村智 on 2019-05-15.
//

#ifndef COSMOSENGINE_EXPORT_H
#define COSMOSENGINE_EXPORT_H

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define COSMOS_ENGINE_DLL_IMPORT __declspec(dllimport)
#define COSMOS_ENGINE_DLL_EXPORT __declspec(dllexport)
#define COSMOS_ENGINE_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define COSMOS_ENGINE_DLL_IMPORT __attribute__ ((visibility ("default")))
#define COSMOS_ENGINE_DLL_EXPORT __attribute__ ((visibility ("default")))
#define COSMOS_ENGINE_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define COSMOS_ENGINE_DLL_IMPORT
#define COSMOS_ENGINE_DLL_EXPORT
#define COSMOS_ENGINE_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define ENGINE_API and ENGINE_LOCAL.
// ENGINE_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// ENGINE_LOCAL is used for non-api symbols.

//#ifdef ENGINE_DLL // defined if ENGINE is compiled as a DLL
#ifdef ENGINE_EXPORTS // defined if we are building the ENGINE DLL (instead of using it)
#define ENGINE_API COSMOS_ENGINE_DLL_EXPORT
#else
#define ENGINE_API COSMOS_ENGINE_DLL_IMPORT
#endif // ENGINE_DLL_EXPORTS
#define ENGINE_LOCAL COSMOS_ENGINE_DLL_LOCAL
//#else // ENGINE_DLL is not defined: this means ENGINE is a static lib.
//#define ENGINE_API
//#define ENGINE_LOCAL
//#endif // ENGINE_DLL


#endif //COSMOSENGINE_EXPORT_H
