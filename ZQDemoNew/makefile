CFLAGS  = -c -g -Wall -D LINUX
all : ZQDemoNew

ZQDemoNew : demo.cpp demo.h
	g++ -o ZQDemoNew -fpermissive -g -Wall -D LINUX demo.cpp -lrt -lt2sdk -L .
clean:
	rm -rf *.o ZQDemoNew
