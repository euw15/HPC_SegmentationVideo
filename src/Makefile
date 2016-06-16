CC=g++
CFLAGS=-I/usr/local/include -Wall -fopenmp
LDFLAGS=-L/usr/local/lib -fopenmp
STATICLIBS=-Wl,-Bstatic
DYNLIBS=-Wl,-Bdynamic -lopencv_core -lopencv_imgcodecs -lopencv_highgui


mapeo_version_papi: SegmentVideo.o
	g++ -L/usr/local/lib -o "SegmentVideo"  ./src/SegmentVideo.o   -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
mapeo_version_papi.o: SegmentVideo.cpp
	g++ -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MT"src/SegmentVideo.o" -o "src/SegmentVideo.o" "src/SegmentVideo.cpp"

clean:
	rm -f SegmentVideo *.o
