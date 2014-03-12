#include "RawFrame.h"
#include "../../AndroidLog.h"

RawFrame::RawFrame(unsigned char * data, int pSize, int w, int h){
	this->dataPtr = data;
	this->pixelSize = pSize;
	this->w = w;
	this->h = h;
}

RawFrame::~RawFrame(){
	if(this->dataPtr != NULL){
		delete this->dataPtr;
	}
}

unsigned char * RawFrame::getData(){
	return this->dataPtr;
}

int RawFrame::getWidth(){
	return this->w;
}

int RawFrame::getHeight(){
	return this->h;
}

int RawFrame::getPixelSize(){
	return this->pixelSize;
}