/*
 * QTAudioRenderer.h
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef QTAUDIORENDERER_H_
#define QTAUDIORENDERER_H_

#include "../../AndroidLog.h"
#include "../libdashframework/Buffer/AudioChunk.h"
#include "AudioFormat.h"
#include "AudioDriverNative.h"

using namespace libdash::framework::buffer;

namespace sampleplayer
{
    namespace renderer
    {
        class DroidAudioRenderer
        {
            public:
                DroidAudioRenderer            ();
                virtual ~DroidAudioRenderer   ();
                void                    WriteToBuffer   (AudioChunk *samples);
                void StartPlayback();
                void StopPlayback();

            private:
                OPENSL_STREAM               *stream;
                // AudioDriverNative               *drv;
        };
    }
}

#endif /* QTAUDIORENDERER_H_ */
