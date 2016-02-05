CFLAGS = -c -g -Wall -D LINUX
INCLUDE = ../Include -I ../Common
all : ZQDemo
ZQDemo : SecuMD.o SecuTrade.o main.o tool.o analyStr.o
	g++ -o ZQDemo -g -Wall -D LINUX libt2sdk.so SecuMD.o SecuTrade.o main.o tool.o -lrt
main.o : main.cpp
	g++ $(CFLAGS) main.cpp  -I $(INCLUDE)
SecuMD.o : SecuMD.cpp
	g++ $(CFLAGS) SecuMD.cpp -I $(INCLUDE)
SecuTrade.o : SecuTrade.cpp
	g++ $(CFLAGS) SecuTrade.cpp -I $(INCLUDE)
tool.o : ../Common/tool.cpp
	g++ $(CFLAGS) ../Common/tool.cpp -I $(INCLUDE)
analyStr.o : ../Common/analyStr.cpp
	g++ $(CFLAGS) -Wno-write-strings ../Common/analyStr.cpp -I $(INCLUDE)
clean:
	rm -rf *.o ZQDemo
