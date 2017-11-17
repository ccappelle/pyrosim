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
CXXFLAGS=-std=c++11 -DHAVE_CONFIG_H -I. -Iode-0.12/ode/src -Iode-0.12/include -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MT -MP
BIN=simulator

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
LIBTOOLOPTS=/bin/bash ode-0.12/libtool --tag=CXX --mode=link
DSFRAMEWORK=ode-0.12/drawstuff/src/libdrawstuff.la ode-0.12/ode/src/libode.la -framework OpenGL -framework GLUT -lm -lpthread ${openglopts}

all: $(OBJ)
	$(LIBTOOLOPTS) $(CXX) -g -O2 -o $(BIN) $(OBJ) $(DSFRAMEWORK)

.cpp.o:
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<


.PHONY: clean
clean:
	rm -f *.o
	rm -f *.d
	rm -f $(BIN)
