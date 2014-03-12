#ifndef _CNATIVE_WRAPPER_H
#define _CNATIVE_WRAPPER_H

#include <jni.h>
#include "DASHInterface.h"
#include <assert.h>
#include "AndroidLog.h"

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class CNativeWrapper{

	private:
    DASHInterface *dashInterface;

	public:
		CNativeWrapper();
		~CNativeWrapper();
    	int Init(JNIEnv * env, jobject * callback, int width, int height);
    	void issueFrameDraw();
    	void resizeViewPort(int, int);
    	void StartWithMPD(const std::string &url);

};

#endif /* _CNATIVE_WRAPPER_H */