#include "AudioFormat.h"

AudioFormat::AudioFormat(){
}

void AudioFormat::setSampleRate(int rate){
	this->sampleRate = rate;
}

void AudioFormat::setChannelCount(int ch){
	this->channels = ch;
}

void AudioFormat::setSampleSize(int s){
	this->sampleSize = s;
}

void AudioFormat::setSampleType(int){}

void AudioFormat::setByteOrder(ByteOrder order){
	this->byteOrder = order;
}

void AudioFormat::setCodec(Codec c){
	this->codec = c;
}

int AudioFormat::getSampleRate(){
	return this->sampleRate;
}

int AudioFormat::getChannelCount(){
	return this->channels;
}

int AudioFormat::getSampleSize(){
	return this->sampleSize;
}

void AudioFormat::setSampleCount(int s){
	this->samples = s;
}

int AudioFormat::getSampleCount(){
	return this->samples;
}
// QAudioFormat *format = new QAudioFormat();
// format->setSampleRate(props->sampleRate);
// format->setChannelCount(props->channels);
// format->setSampleSize(16);
// format->setCodec("audio/pcm");
// format->setByteOrder(QAudioFormat::LittleEndian);
// format->setSampleType(QAudioFormat::SignedInt);