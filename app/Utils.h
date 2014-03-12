#ifndef _HR_UTILS_
#define _HR_UTILS_
#include <sys/time.h>

int64_t getCurrentMicros(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}

int64_t getCurrentMillis(){
	return getCurrentMicros() / 1000;
}

int64_t getCurretSeconds(){
	return getCurrentMillis() / 1000;
}
#endif