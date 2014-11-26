VPATH=Source

CXXFLAGS=-g -std=c++11 -Wall
SDL2FLAGS= -D_REENTRANT -I/usr/local/include/SDL2  -Wl,-rpath,/usr/local/lib -L/usr/local/lib -lSDL2 -lpthread
GLFLAGS= -lGL

main: main.cpp
	$(CXX) $(CXXFLAGS) Source/main.cpp -o main $(SDL2FLAGS) $(GLFLAGS)

.PHONY: clean
clean:
	$(RM) main
