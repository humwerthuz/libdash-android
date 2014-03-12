SOURCE_FILES = $(shell find . | grep .cpp)

SOURCES=$(shell find . -type f -printf "%f\n" | grep .cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

LIBS= -lavcodec-55.34.1 -lavfilter-4.1.0 -ldash \
-lswscale-2.1.2 -lavformat-55.12.0 -llog -landroid -lEGL -lGLESv1_CM \
-lyuv2rgb -lOpenSLES -lavresample-1.1.0 -lavutil-53.3.0

HEADERS=-IDecoder -IManagers -IRenderer -Ilibdashframework/Adaptation \
-Ilibdashframework/Buffer -Ilibdashframework/helpers -Ilibdashframework/Input \
-Ilibdashframework/MPD -Ilibdashframework/Portable -I../libdash/include \
-I../libs/curl/include -I../libs/libxml2/include \
-I/home/humberto/playground/libdash_android/libs/build/include

LDFLAGS=-L../lib

all: dashiface
	rm -f ../lib/libdashiface.so
	cp libdashiface.so ../lib/libdashiface.so

dashiface.o:
	arm-linux-androideabi-g++ -fpic $(HEADERS) -DGL_GLEXT_PROTOTYPES -c $(SOURCE_FILES) 

dashiface: dashiface.o
	rm -f libdashiface.so
	arm-linux-androideabi-g++ -shared -Wl,--no-undefined $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(LIBS) -o libdashiface.so
	rm -f *.o


clean:
	rm -f *.so *.
