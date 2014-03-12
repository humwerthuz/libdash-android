LIBDASH_FILES = $(shell find source/ | grep .cpp)

SOURCES=$(shell find source/ -type f -printf "%f\n" | grep .cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

all: libdash

libdash.o: 
	$(CXX) -fpic $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $(LIBDASH_FILES)

libdash: libdash.o
	$(CXX)  -shared $(OBJECTS) -o libdash.so $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -lxml2 -liconv -lz -lcurl

install:
	cp libdash.so ../lib/

clean:
	rm *.o
