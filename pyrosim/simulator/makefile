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
CXXFLAGS=-std=c++11 -DHAVE_CONFIG_H -DdTRIMESH_ENABLED -DdDOUBLE  -g -O2 -MMD -MP

SRC_DIR=src
# INC_DIR=include
INC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj
ODEDIR=external/ode-0.12
BIN=simulator

# necessary for linking to ode
LIBTOOLOPTS=/bin/bash $(ODEDIR)/libtool --tag=CXX --mode=link
# necessary for linking to drawstuff
DSFRAMEWORK=$(ODEDIR)/drawstuff/src/libdrawstuff.la $(ODEDIR)/ode/src/libode.la -framework OpenGL -framework GLUT -lm -lpthread ${openglopts}

# get source names with stripped directory
SRCS := $(notdir $(shell find $(SRC_DIR) -name *.cpp))

# OTHERSRCS := $(wildcard src/*/*.cpp) 
#$(SRC_DIR)/*.cpp */*/*.cpp)


# get obj names from src names
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
# get dependencies from obj
DEPS := $(OBJS:.o=.d)


$(info $(SRCS))
$(info $(SRC_DIR))
$(info $(OBJS))
$(info $(DEPS))

INCL_FLAGS := -I$(INC_DIR) -I$(ODEDIR)/include -I$(ODEDIR)/ode/src

$(BUILD_DIR)/$(BIN): $(OBJS)
	$(LIBTOOLOPTS) $(CXX) $(INCL_FLAGS) $(OBJS) -o $@ $(DSFRAMEWORK)

$(OBJ_DIR)/%.cpp.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCL_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)

