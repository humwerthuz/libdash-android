#ifndef _RAWFRAME_H
#define _RAWFRAME_H

class RawFrame{
public:
	RawFrame(unsigned char * data, int pSize, int w, int h);
	~RawFrame();
	unsigned char * getData();
	int getWidth();
	int getHeight();
	int getPixelSize();
private:
	unsigned char * dataPtr;
	int pixelSize;
	int w;
	int h;
};

#endif /* _RAWFRAME_H */