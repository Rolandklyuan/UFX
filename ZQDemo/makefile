CFLAGS = -c -D LINUX
all : ZQDemo
ZQDemo : SecuMD.o SecuTrade.o main.o tool.o analyStr.o
	g++ -o ZQDemo -D LINUX demo/libt2sdk.so SecuMD.o SecuTrade.o main.o tool.o -lrt
main.o : demo/main.cpp
	g++ -c demo/main.cpp -D LINUX -I Include -I Common
SecuMD.o : demo/SecuMD.cpp
	g++ $(CFLAGS) demo/SecuMD.cpp -I Include -I Common
SecuTrade.o : demo/SecuTrade.cpp
	g++ $(CFLAGS) demo/SecuTrade.cpp -I Include -I Common
tool.o : Common/tool.cpp
	g++ $(CFLAGS) Common/tool.cpp  -I Include -I Common
analyStr.o : Common/analyStr.cpp
	g++ $(CFLAGS) Common/analyStr.cpp -I Include -I Common
clean:
	rm -rf *.o ZQDemo
