CXX=g++
CXXFLAGS = -std=c++11 -Wall -g
DEPS = vec3.hpp ray.hpp

main: main.o
	$(CXX) -o main $(CXXFLAGS) main.o

test: test.o
	$(CXX) -o test $(CXXFLAGS) test.o

main.o: $(DEPS)
test.o: $(DEPS) catch.hpp

clean:
	rm *.o
