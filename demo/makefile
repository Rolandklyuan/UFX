all : demo
demo: demo.cpp demo.h t2sdk_interface.h
	g++ -o demo -std=c++11 -g -Wall -fpermissive demo.cpp libt2sdk.so -I . -lpthread
#g++ -o demo -std=c++11 -g -Wall -fpermissive demo.cpp libt2sdk.so -I . -lpthread
clean:
	rm -rf *.o demo
