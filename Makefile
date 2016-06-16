CC=g++
CFLAGS=-I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP
LDFLAGS=-L/usr/local/lib -fopenmp
STATICLIBS=-Wl,-Bstatic
DYNLIBS=-lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_videoio


videoLinker: src/SegmentVideo.o
	g++ $(LDFLAGS) -o "SegmentVideo" ./src/SegmentVideo.o $(DYNLIBS)

videoObject: src/SegmentVideo.cpp
	g++  $(CFLAGS)  -MT"src/SegmentVideo.o" -o "src/SegmentVideo.o" "src/SegmentVideo.cpp"

clean:
	rm -f SegmentVideo *.o
