	g++ -std=c++11 -c -g Application.cpp Task.cpp general.cpp Vertex.cpp w_simulator.cpp Clock.cpp  Map.cpp Core.cpp 
	g++ -g -o simple Application.o w_simulator.o general.o Vertex.o Task.o Clock.o Map.o Core.o
