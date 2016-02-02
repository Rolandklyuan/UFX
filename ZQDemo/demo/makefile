all : ZQDemo
ZQDemo : SecuMD.o SecuTrade.o main.o tool.o analyStr.o
	g++ -o ZQDemo -D LINUX libt2sdk.so SecuMD.o SecuTrade.o main.o tool.o -lrt
main.o : main.cpp
	g++ -c main.cpp -D LINUX -I ../Include -I ../Common
SecuMD.o : SecuMD.cpp
	g++ -c SecuMD.cpp -D LINUX -I ../Include -I ../Common
SecuTrade.o : SecuTrade.cpp
	g++ -c SecuTrade.cpp -D LINUX  -I ../Include -I ../Common
tool.o : ../Common/tool.cpp
	g++ -c ../Common/tool.cpp -D LINUX  -I ../Include -I ../Common
analyStr.o : ../Common/analyStr.cpp
	g++ -c ../Common/analyStr.cpp -D LINUX  -I ../Include -I ../Common
clean:
	rm -rf *.o ZQDemo
