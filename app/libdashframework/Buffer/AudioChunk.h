/*
 * AudioChunk.h
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#ifndef QTSAMPLEPLAYER_BUFFER_AUDIOCHUNK_H_
#define QTSAMPLEPLAYER_BUFFER_AUDIOCHUNK_H_

#include "config.h"

namespace libdash
{
    namespace framework
    {
        namespace buffer
        {
            class AudioChunk
            {
                public:
                    AudioChunk          (void *format, char * data, uint64_t dataLength);
                    virtual ~AudioChunk ();

                    char*      Data    ();
                    uint64_t            Length  ();
                    void*               Format  ();

                private:
                    uint64_t     chunkLength;
                    char         *data;
                    void         *format;
            };
        }
    }
}

#endif /* QTSAMPLEPLAYER_BUFFER_AUDIOCHUNK_H_ */
