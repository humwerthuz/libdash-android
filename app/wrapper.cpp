#include "com_wannabeapp_android_libdash_DASHInstance.h"
#include "CNativeWrapper.h"

extern "C" {

#define CAST_HANDLE(wr) jclass c = env->GetObjectClass(obj);\
                        if (!c) return;\
                        jfieldID fid = env->GetFieldID(c, "handle", "J");\
                        if (!fid){\
                          jniLOGE("No Field ID, ERROR");\
                          return;\
                        }\
                        jlong __h = env->GetLongField(obj, fid);\
                        CNativeWrapper* wr = (CNativeWrapper*) __h;

/*
 * Class:     com_wannabeapp_android_libdash_DASHInstance
 * Method:    createInstance
 * Signature: (Lcom/wannabeapp/android/libdash/DASHCallback;II)J
 */
JNIEXPORT jlong JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_createInstance(JNIEnv * env, jobject obj, jobject callback, jint width, jint height){
  CNativeWrapper * dash_obj = new CNativeWrapper();
  if(dash_obj){
    int w = width;
    int h = height;
    jniLOGI("Calling dash_obj->init()...");
    if(dash_obj->Init(env, &callback, w, h)){
      jniLOGE("Failed to init(), return code not 0");
    }
  } else {
    jniLOGE("Failed to create new CNativeWrapper instance");
  }
  return (jlong) dash_obj;
}

/*
 * Class:     com_wannabeapp_android_libdash_DASHInstance
 * Method:    disconnect
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_disconnect(JNIEnv * env, jobject obj){
	return;
}

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_startWithMPD(JNIEnv * env, jobject obj, jstring url){
  CAST_HANDLE(wr);
  wr->StartWithMPD(std::string(env->GetStringUTFChars(url, NULL)));
}

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_renderFrame(JNIEnv * env, jobject obj){
  // LOGD("Received call to renderFrame [native]");
  CAST_HANDLE(wr);
  wr->issueFrameDraw();
}

JNIEXPORT void JNICALL Java_com_wannabeapp_android_libdash_DASHInstance_resize(JNIEnv * env, jobject obj, jint w, jint h){
  LOGD("Received call to resize [native]");
  CAST_HANDLE(wr);
  wr->resizeViewPort((int) w, (int) h);
}
}