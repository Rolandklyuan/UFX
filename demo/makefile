all : demo
demo: demo.cpp t2sdk_interface.h
	g++ -o demo -std=c++11 -g -Wall -fpermissive demo.cpp libt2sdk.so -I . -lpthread
clean:
	rm -rf *.o demo
