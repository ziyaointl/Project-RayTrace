CXX=g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
DEPS = vec3.hpp ray.hpp sphere.hpp hittable.hpp

main: main.o
	$(CXX) -o main $(CXXFLAGS) main.o

test: test.o
	$(CXX) -o test $(CXXFLAGS) test.o

main.o: $(DEPS)
test.o: $(DEPS) catch.hpp

clean:
	rm *.o
