/*
 * DASHManager.cpp
 *****************************************************************************
 * Copyright (C) 2012, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "DASHManager.h"
#include "../../Renderer/AudioFormat.h"
#include "../../AndroidLog.h"
#include "../../yuv2rgb.h"

using namespace libdash::framework::input;
using namespace libdash::framework::buffer;
using namespace sampleplayer::decoder;

using namespace dash;
using namespace dash::network;
using namespace dash::mpd;


/******** TESTING AREA **********/

/********************************/

DASHManager::DASHManager        (uint32_t maxCapacity, IDASHManagerObserver* stream, IMPD* mpd) :
             readSegmentCount   (0),
             receiver           (NULL),
             mediaObjectDecoder (NULL),
             multimediaStream   (stream),
             isRunning          (false)
{
    this->buffer    = new MediaObjectBuffer(maxCapacity);
    this->buffer->AttachObserver(this);

    this->receiver  = new DASHReceiver(mpd, this, this->buffer, maxCapacity);
}
DASHManager::~DASHManager       ()
{
    this->Stop();
    delete this->receiver;
    delete this->buffer;

    this->receiver = NULL;
    this->buffer   = NULL;
}

bool        DASHManager::Start                  ()
{
    if (!this->receiver->Start()){
        LOGE("Failed to start DASH receiver");
        return false;
    }

    if (!this->CreateAVDecoder()){
        LOGE("Failed to create AV decoder");
        return false;
    }

    this->isRunning = true;
    return true;
}
void        DASHManager::Stop                   ()
{
    if (!this->isRunning)
        return;

    this->isRunning = false;

    this->receiver->Stop();
    this->mediaObjectDecoder->Stop();
    this->buffer->Clear();
}
uint32_t    DASHManager::GetPosition            ()
{
    return this->receiver->GetPosition();
}
void        DASHManager::SetPosition            (uint32_t segmentNumber)
{
    this->receiver->SetPosition(segmentNumber);
}
void        DASHManager::SetPositionInMsec      (uint32_t milliSecs)
{
    this->receiver->SetPositionInMsecs(milliSecs);
}
void        DASHManager::Clear                  ()
{
    this->buffer->Clear();
}
void        DASHManager::ClearTail              ()
{
    this->buffer->ClearTail();
}
void        DASHManager::SetRepresentation      (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
    this->receiver->SetRepresentation(period, adaptationSet, representation);
    //this->buffer->ClearTail();
}
void        DASHManager::EnqueueRepresentation  (IPeriod *period, IAdaptationSet *adaptationSet, IRepresentation *representation)
{
    this->receiver->SetRepresentation(period, adaptationSet, representation);
}

/****  THIS ONE WORKS, BUT IS FRACKING SLOW ****/
// void        DASHManager::OnVideoFrameDecoded    (const uint8_t **data, videoFrameProperties* props)
// {
//     /* TODO: some error handling here */
//     if (data == NULL || props->fireError){
//         LOGE("Ups.. bad frame. Discarding...");
//         return;
//     }

//     int w = props->width;
//     int h = props->height;

//     AVFrame *rgbframe   = avcodec_alloc_frame();
//     int     numBytes    = avpicture_get_size(PIX_FMT_RGB24, w, h);
//     uint8_t *buffer     = (uint8_t*)av_malloc(numBytes);


//     avpicture_fill((AVPicture*)rgbframe, buffer, PIX_FMT_RGB24, w, h);

//     SwsContext *imgConvertCtx = sws_getContext(props->width, props->height, (PixelFormat)props->pxlFmt, w, h, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

//     sws_scale(imgConvertCtx, data, props->linesize, 0, h, rgbframe->data, rgbframe->linesize);

//     int frameSize = 3 * w * h; // each pixel is 24 bits in size (RGB888)

//     unsigned char * outputBuffer = new unsigned char[frameSize]; 

//     memcpy(outputBuffer, (uint8_t *)rgbframe->data[0], frameSize);

//     this->multimediaStream->AddFrame(new RawFrame(outputBuffer, w, h));

//     av_free(rgbframe);
//     av_free(buffer);
// }
/*** THIS ONE GIVES A KICK IN THE BUTT TO THE PREVIOUS METHOD ***/
void DASHManager::OnVideoFrameDecoded(const uint8_t **data, videoFrameProperties* props){
    int w = props->width;
    int h = props->height;

    unsigned char * rgba = new unsigned char[w * h * 4];
    AVFrame *frame = (AVFrame*)(*data);

    yuv420_2_rgb8888(rgba,
                    frame->data[0],
                    frame->data[2],
                    frame->data[1],
                    w,
                    h,
                    frame->linesize[0],
                    frame->linesize[1],
                    w * 4,
                    yuv2rgb565_table,
                    0);

    this->multimediaStream->AddFrame(new RawFrame(rgba, 4, w, h)); // 4 -> pixel size
}
void        DASHManager::OnAudioSampleDecoded   (int16_t * buffer, audioFrameProperties* props)
{
    /* TODO: some error handling here */
    if (buffer == NULL || props->fireError)
        return;

    AudioFormat *format = new AudioFormat();
    format->setSampleRate(props->sampleRate);
    format->setSampleCount(props->samples);
    format->setChannelCount(props->channels);
    format->setSampleSize(16);
    format->setCodec(AUDIO_PCM);
    format->setByteOrder(LITTLE_ENDIAN);
    //format->setSampleType();

    AudioChunk *samples = new AudioChunk(format, (char*)buffer, props->linesize);

    this->multimediaStream->AddSamples(samples);
}
void        DASHManager::OnBufferStateChanged   (uint32_t fillstateInPercent)
{
    this->multimediaStream->OnSegmentBufferStateChanged(fillstateInPercent);
}
void        DASHManager::OnSegmentDownloaded    ()
{
    this->readSegmentCount++;

    // notify observers
}
void        DASHManager::OnDecodingFinished     ()
{
    if (this->isRunning)
        this->CreateAVDecoder();
}
bool        DASHManager::CreateAVDecoder        ()
{
    MediaObject *mediaObject            = this->buffer->GetFront();

    // initSegForMediaObject may be NULL => BaseUrls
    if (!mediaObject){
        LOGE("Failed to fetch MediaObject");
        return false;
    }

    MediaObject *initSegForMediaObject  = this->receiver->FindInitSegment(mediaObject->GetRepresentation());

    this->mediaObjectDecoder = new MediaObjectDecoder(initSegForMediaObject, mediaObject, this);
    return this->mediaObjectDecoder->Start();
}
