CXX := g++ 
CXXFLAGS := -g -O0 -std=c++11

SRC	:= p2.cpp Scheduler.cpp
HDR	:= Scheduler.h
OBJ	:= $(patsubst %.cpp, %.o, $(SRC))

all: sim

sim: $(OBJ)
	$(CXX) -o $@ $(OBJ)

%.o:	%.cpp $(HDR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


