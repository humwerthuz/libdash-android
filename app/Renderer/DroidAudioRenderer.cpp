/*
 * DroidAudioRenderer.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *         
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include <iostream>
#include "DroidAudioRenderer.h"
#include "../libdashframework/Portable/MultiThreading.h"
using namespace sampleplayer::renderer;

DroidAudioRenderer::DroidAudioRenderer(){
	// this->drv = NULL;
	this->stream = NULL;
}

DroidAudioRenderer::~DroidAudioRenderer(){}

void DroidAudioRenderer::StartPlayback(){}
void DroidAudioRenderer::StopPlayback(){}

// void DroidAudioRenderer::WriteToBuffer(AudioChunk *samples){
// 	AudioFormat *format = (AudioFormat *) samples->Format();

// 	if(this->drv == NULL){
// 		this->drv = new AudioDriver();
// 	}

// 	int outBufferLen =  samples->Length();

// 	char * outBuffer = new char[outBufferLen];

// 	memcpy(outBuffer, samples->Data(), samples->Length());

// 	drv->WriteToBuffer(outBuffer, outBufferLen);
// 	delete outBuffer;

// 	/** We have to wait for the samples to be played to enqueue another **/
// 	float wait_time = ((float)format->getSampleCount()) / format->getSampleRate();
// 	PortableSleep(wait_time);
// }

void DroidAudioRenderer::WriteToBuffer(AudioChunk *samples){
	AudioFormat *format = (AudioFormat *) samples->Format();

	if(this->stream == NULL){
		this->stream = android_OpenAudioDevice(format->getSampleRate(),
										       0,
											   format->getChannelCount(),
											   1024*2);
	} 
	// else if(this->contextChanged(format)){
	// 	audioDevice.updateParams();
	// }

	int outBufferLen =  samples->Length();

	char * outBuffer = new char[outBufferLen];

	memcpy(outBuffer, samples->Data(), samples->Length());

	android_AudioOut(this->stream, outBuffer, samples->Length());
	// delete outBuffer;

	/** We have to wait for the samples to be played to enqueue another **/
	/** OTHERWISE OUR BUFFERS WILL FILL AND WILL NOT RECEIVE SHIT  **/
	float wait_time = ((float)format->getSampleCount()) / format->getSampleRate();

	PortableSleep(wait_time / 2);
}