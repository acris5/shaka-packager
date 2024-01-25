//#pragma once
#ifndef PACKAGER_PACKAGER_EXT_H_
#define PACKAGER_PACKAGER_EXT_H_
#ifdef __cplusplus
extern "C" {
#endif

__attribute__((visibility("default"))) void* Packager_New();
__attribute__((visibility("default"))) int Packager_Initialize(
    void* packager,
    const char* json_args);
__attribute__((visibility("default"))) void Packager_Destroy(void* packager);
__attribute__((visibility("default"))) int Packager_Run(void* packager);
__attribute__((visibility("default"))) void Packager_Stop(void* packager);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif