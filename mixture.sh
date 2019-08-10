	g++ -std=c++11 -c -g Application.cpp Task.cpp general.cpp Vertex.cpp  Clock.cpp  Map.cpp Core.cpp Event.cpp Region.cpp global.cpp  main.cpp
	g++ -g -o mixture Application.o  general.o Vertex.o Task.o Clock.o Map.o Core.o Event.o Region.o global.o  main.o
