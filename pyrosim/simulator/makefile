UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	openglopts = "-lGL -lGLU -lglut"
	libtoolshell = "/bin/bash"
endif
ifeq ($(UNAME), Darwin)
	openglopts = ""
	libtoolshell = "/bin/sh"
endif

CXX=g++
CXXFLAGS=-std=c++11 -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src  -Iode-0.12/include -DDRAWSTUFF_TEXTURE_PATH="\"ode-0.12/drawstuff/textures\"" -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT -MD -MP 
BIN=simulator

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
PREOPTS=/bin/bash ode-0.12/libtool --tag=CXX --mode=link
POSTOPTS=ode-0.12/drawstuff/src/libdrawstuff.la ode-0.12/ode/src/libode.la -framework OpenGL -framework GLUT -lm -lpthread ${openglopts}

all: $(OBJ)
	$(PREOPTS) $(CXX) -g -O2 -o $(BIN) $(OBJ) $(POSTOPTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

clean:
	rm -f *.o
	rm $(BIN)
