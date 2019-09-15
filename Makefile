UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CXX=g++-9
else
CXX=g++
endif

CXXFLAGS=-std=c++11 -Wall -Wextra -g -fopenmp -lpthread
DEPS=vec3.hpp ray.hpp sphere.hpp hittable.hpp hittable_list.hpp random.hpp camera.hpp material.hpp

main: main.o
	$(CXX) -o main $(CXXFLAGS) main.o

test: test.o test-include.o
	$(CXX) -o test $(CXXFLAGS) test.o test-include.o

main.o: $(DEPS)
test.o: $(DEPS) catch.hpp
test-include.o: catch.hpp

clean:
	rm *.o
