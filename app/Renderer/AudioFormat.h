#ifndef _AUDIO_FORMAT_H
#define _AUDIO_FORMAT_H

enum ByteOrder{
	LITTLE_ENDIAN,
	BIG_ENDIAN
};

enum Codec{
	AUDIO_PCM
};

class AudioFormat{
private:
	int sampleRate;
	int channels;
	int sampleSize;
	int byteOrder; 	// 1 -> littleendian, 0 -> big
	int sampleType;
	int samples;
	Codec codec;

public:
	AudioFormat();
	void setSampleRate(int);
	void setChannelCount(int);
	void setSampleSize(int);
	void setByteOrder(ByteOrder);
	void setSampleType(int);
	void setCodec(Codec);
	void setSampleCount(int);
	int getSampleCount();
	int getSampleRate();
	int getChannelCount();
	int getSampleSize();
	ByteOrder getByteOrder();
	Codec getCodec();


};

#endif