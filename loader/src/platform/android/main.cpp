#include <Sapphire/DefaultInclude.hpp>

#if defined(GEODE_IS_ANDROID)

#include "../load.hpp"
#include <jni.h>

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    sapphireEntry(nullptr);
    return JNI_VERSION_1_1;
}

#endif