/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_wannabeapp_android_libdash_DASHInstance */

#ifndef _Included_com_wannabeapp_android_libdash_DASHInstance
#define _Included_com_wannabeapp_android_libdash_DASHInstance
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_wannabeapp_android_libdash_DASHInstance
 * Method:    createInstance
 * Signature: (Lcom/wannabeapp/android/libdash/DASHCallback;II)J
 */
JNIEXPORT jlong JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_createInstance
  (JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     com_wannabeapp_android_libdash_DASHInstance
 * Method:    disconnect
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_disconnect
  (JNIEnv *, jobject);


JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_startWithMPD
  (JNIEnv *, jobject, jstring);

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_renderFrame
  (JNIEnv *, jobject);

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_resize
  (JNIEnv *, jobject, jint, jint);
#ifdef __cplusplus
}
#endif
#endif
