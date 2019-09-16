# Project Raytrace
Consists of a C++ distributed ray-tracer and a Python path-tracer.

The C++ ray-tracer is in the root directory. It is based on Peter Shirley's wonderful book(s) at [https://raytracing.github.io/](https://raytracing.github.io/).

The Python path-tracer is in the cs61a folder. It is based on [smallpt](https://www.kevinbeason.com/smallpt/).

![Sample Output](sample.png)

### Usage
Instructions below apply to the C++ version only.

1. To compile
```bash
make
```
Note: OpenMP required

2. To run
```bash
./main > output.ppm
```

3. (Optional) Compile and run unit tests
```bash
make test
./test
```
