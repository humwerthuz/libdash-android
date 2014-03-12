/*
 * DroidGLRenderer.h
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef DROIDGLRENDERER_H_
#define DROIDGLRENDERER_H_

#include "../libdashframework/Portable/MultiThreading.h"
#include "../libdashframework/Buffer/RawFrame.h"
#include "../AndroidLog.h"
 
#include <GLES/gl.h>
#include <GLES/glext.h>

namespace sampleplayer
{
    namespace renderer
    {
        class DroidGLRenderer
        {

            public:
                DroidGLRenderer();
                ~DroidGLRenderer();

                void update();
                void Init(int, int);
                void SetImage(RawFrame  * frame);
                void resizeViewPort(int, int);

            private:
                int fh;
                int fw;
                int width;
                int height;
                int tex_width;
                int tex_height;
                GLubyte *textureBuffer;
                bool iHaveSomethingToDraw;

                GLuint  texture;
                CRITICAL_SECTION    monitorMutex;
                CRITICAL_SECTION    monitorVsync;

            private:
                void initGL();
                void resizeWindow();
                void drawGLScene();
                void createTexture();
                void releaseTexture();
                void updateTextureBuffer(RawFrame * frame);
        };
    }
}

#endif /* DROIDGLRENDERER_H_ */
