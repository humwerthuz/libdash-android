#ifndef _ANDROID_LOG_H_CUSTOM
#define _ANDROID_LOG_H_CUSTOM
#include <android/log.h>

#define jniTAG "WRAPPER_JNI"
#define TAG "LIBDASH_ANDROID_NATIVE"

#define jniLOGV(X)  __android_log_print(ANDROID_LOG_VERBOSE, jniTAG, X)
#define jniLOGI(X)  __android_log_print(ANDROID_LOG_INFO, jniTAG, X)
#define jniLOGE(X)  __android_log_print(ANDROID_LOG_ERROR, jniTAG, X)
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif /*_ANDROID_LOG_H_CUSTOM*/