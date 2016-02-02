all : demo
demo: demo.cpp demo.h t2sdk_interface.h
	g++ -o demo -std=c++11 -g -Wall demo.cpp libt2sdk.so
clean:
	rm -rf *.o demo
