CC=g++
CFLAGS=-I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP
LDFLAGS=-L/usr/local/lib -fopenmp
STATICLIBS=-Wl,-Bstatic
DYNLIBS=-lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_videoio


videoLinker: 
	g++ -L/usr/local/lib -o TemporalSegmentation  src/GraphUtils.o src/TemporalSegmentation.o -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_core

videoObject: src/SegmentVideo.cpp
	g++ -I/usr/local/include/opencv -O0 -g3 -Wall -c -o src/TemporalSegmentation.o src/SegmentVideo.cpp

clean:
	rm -f SegmentVideo *.o
