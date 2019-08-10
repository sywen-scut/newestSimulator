	g++ -std=c++11 -c -g  Application.cpp Task.cpp generator.cpp general.cpp Vertex.cpp 
	g++ -g -o generate Application.o Task.o generator.o general.o Vertex.o
