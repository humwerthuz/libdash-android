
#include "CNativeWrapper.h"
#include <android/native_window_jni.h>

CNativeWrapper::CNativeWrapper(){
	// mainJavaEnv = NULL;
}

CNativeWrapper::~CNativeWrapper(){
      // JavaEnvTh * env = getEnv();
      // if (env && env->cbk_obj)
      //   env->env->DeleteGlobalRef(env->cbk_obj);

}

int CNativeWrapper::Init(JNIEnv * env, jobject * callback, int width, int height){

  // if(!mainJavaEnv){
  //   mainJavaEnv = (JavaEnvTh *) malloc(sizeof(JavaEnvTh));
  // }

  // memset(mainJavaEnv, 0, sizeof(JavaEnvTh));
  // setJavaEnv(mainJavaEnv, env, env->NewGlobalRef(*callback));

  // if (pthread_setspecific( jni_thread_env_key, mainJavaEnv)){
  //   LOGE("Failed to set specific thread data to jni_thread_env_key=%p for main thread !", jni_thread_env_key);
  // }
  LOGD("Starting DASH interface");
  this->dashInterface = new DASHInterface();
  LOGD("Initializing GL Renderer");
  this->dashInterface->InitDroidGLRenderer(width, height);
  return 0;
}

void CNativeWrapper::StartWithMPD(const std::string &url){
  LOGI(("Starting with url " + url).c_str());
  if(this->dashInterface->DownloadMPD(url)){
    LOGD(("Successfully parsed MPD at: " + url).c_str());
  } else {
    LOGE(("Error parsing MPD at: " + url).c_str());
  }
  LOGD(("Attemptimg playback at: " + url).c_str());
  this->dashInterface->Start();
}

void CNativeWrapper::issueFrameDraw(){
  this->dashInterface->issueFrameDraw();
}

void CNativeWrapper::resizeViewPort(int w, int h){
  this->dashInterface->resizeViewPort(w, h);
}