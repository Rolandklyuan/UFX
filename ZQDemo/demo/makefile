CFLAGS = -c -D LINUX
all : ZQDemo
ZQDemo : SecuMD.o SecuTrade.o main.o tool.o analyStr.o
	g++ -o ZQDemo -D LINUX libt2sdk.so SecuMD.o SecuTrade.o main.o tool.o -lrt
main.o : main.cpp
	g++ -c main.cpp -D LINUX -I ../Include -I ../Common
SecuMD.o : SecuMD.cpp
	g++ $(CFLAGS) SecuMD.cpp -I ../Include -I ../Common
SecuTrade.o : SecuTrade.cpp
	g++ $(CFLAGS) SecuTrade.cpp -I ../Include -I ../Common
tool.o : ../Common/tool.cpp
	g++ $(CFLAGS) ../Common/tool.cpp  -I ../Include -I ../Common
analyStr.o : ../Common/analyStr.cpp
	g++ $(CFLAGS) -Wwrite-strings ../Common/analyStr.cpp -I ../Include -I ../Common
clean:
	rm -rf *.o ZQDemo
