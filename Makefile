CXX			:= g++ -std=c++11
CXXFLAGS 	:= -Wpedantic -Wall -Wextra -Wuninitialized -O3 -g
TARGET		:= fibheap

all: $(TARGET)

$(TARGET): main.cpp pheap.h pheap.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $<

clean:
	@rm -f $(TARGET)
